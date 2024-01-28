#include "../include/JsonLoader.h"


bool JsonLoader::insertJsonToDatabase(DeviceInfo deviceInfo) {

    try {
        DatabaseControls db_controls;
        deviceInfo.id = db_controls.findExistingIp(deviceInfo.ip_address).value_or(0); 

        if (deviceInfo.id != 0) {
            return updateJsonInDatabase(deviceInfo);
        }

        DatabaseHandler& db_json = DatabaseSingleton::getInstance();
        std::int32_t new_id = db_controls.idFind().value_or(0) + 1;

        // Create a BSON document from the provided data and include the ID
        bsoncxx::builder::stream::document doc_builder;
        doc_builder << "_id" << new_id
                    << "Device name" << deviceInfo.device_name
                    << "IP Address" << deviceInfo.ip_address
                    << "Status" << "Online"
                    << "OS" << deviceInfo.os_type
                    << "Version" << deviceInfo.app_version;

        auto bson_doc = doc_builder << bsoncxx::builder::stream::finalize;

        if (db_json.insertDocument(db_json.getDatabase(), db_json.getCollectionClient(), bson_doc.view())) {
            std::cout << "JSON data inserted into the database with ID: " << new_id << std::endl;
            return true;
        } else {
            std::cerr << "Failed to insert JSON data into the database." << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error interacting with the database: " << e.what() << std::endl;
        return false;
    }
}

bool JsonLoader::updateJsonInDatabase(DeviceInfo deviceInfo) {
    try {
        DatabaseHandler& db_json = DatabaseSingleton::getInstance();
        DatabaseControls db_controls;
        auto coll = db_json.getClient()[db_json.getDatabase()][db_json.getCollectionClient()];

        bsoncxx::builder::stream::document filter_builder;
        filter_builder << "_id" << deviceInfo.id;
        auto filter = filter_builder << bsoncxx::builder::stream::finalize;

        // Create a BSON document with the new field values.
        bsoncxx::builder::stream::document update_builder;
        update_builder << "$set" << bsoncxx::builder::stream::open_document;
        bool hasUpdates = false;

        // Check if the new field values are different from the existing values.
        auto existingDocument = coll.find_one(filter.view());
        if (existingDocument) {
            auto existingDocView = existingDocument->view();
            auto device_name_existing = existingDocView["Device name"].get_utf8().value.to_string();
            auto os_type_existing = existingDocView["OS"].get_utf8().value.to_string();
            auto app_version_existing = existingDocView["Version"].get_utf8().value.to_string();

            if (deviceInfo.device_name != device_name_existing) {
                update_builder << "Device name" << deviceInfo.device_name;
                hasUpdates = true;
            }

            if (deviceInfo.os_type != os_type_existing) {
                update_builder << "OS" << deviceInfo.os_type;
                hasUpdates = true;
            }

            if (deviceInfo.app_version != app_version_existing) {
                update_builder << "Version" << deviceInfo.app_version;
                hasUpdates = true;
            }

            if (!hasUpdates) {
                std::cout << "No changes to update for ID: " << deviceInfo.id << std::endl;
                update_builder << "Status" << "Online" << bsoncxx::builder::stream::close_document;
                auto update = update_builder << bsoncxx::builder::stream::finalize;
                auto result = coll.update_one(filter.view(), update.view());
                return true;
            }
        }

        update_builder << "Status" << "Online" << bsoncxx::builder::stream::close_document;
        auto update = update_builder << bsoncxx::builder::stream::finalize;
        auto result = coll.update_one(filter.view(), update.view());

        if (result && result->modified_count() > 0) {
            std::cout << "JSON data updated in the database for ID: " << deviceInfo.id << std::endl;
            return true;
        } else {
            std::cerr << "Failed to update JSON data in the database." << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error interacting with the database: " << e.what() << std::endl;
        return false;
    }
}
