#include "../include/DatabaseControls.h"



void DatabaseControls::handleRequest(const RequestType type, const std::int32_t& deviceId) {
    using namespace bsoncxx::builder::basic;
    
    DatabaseHandler& dbControls = DatabaseSingleton::getInstance();
    auto coll = dbControls.getClient()[dbControls.getDatabase()][dbControls.getCollectionClient()];

    std::string field;
    StatusType newStatus = StatusType::Unspecified;
    std::string fieldContent;

    switch (type) {
        case RequestType::Shutdown:
            field = "Status";
            newStatus = StatusType::Offline;
            break;
        case RequestType::Restart:
            field = "Status";
            newStatus = StatusType::Offline;
            break;   
        default:
            field = "Invalid";
            break;
    }

    switch (newStatus) {
    case StatusType::Online:
        fieldContent = "Online";
        break;
    case StatusType::Offline:
        fieldContent = "Offline";
        break;    
    default:
        fieldContent = "Invalid";
        break;
    }

    try {
        // Create a BSON document for the query filter
        document query;
        query.append(kvp("_id", deviceId));

        // Create a BSON document for the update operation
        document update;
        update.append(kvp("$set", make_document(kvp(field, fieldContent))));

        // Perform the update
        coll.update_one(query.view(), update.view());
        std::cout << "Updated " << field << " of device with id: " << std::to_string(deviceId) << " to: " << fieldContent << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error updating device in the database: " << e.what() << std::endl;
    }            
}

std::optional<int32_t> DatabaseControls::findExistingIp(const std::string& ip_address) {
    try {
        DatabaseHandler& db_json = DatabaseSingleton::getInstance();
        auto coll = db_json.getClient()[db_json.getDatabase()][db_json.getCollectionClient()];
        
        bsoncxx::builder::stream::document filter_builder;
        filter_builder << "IP Address" << ip_address;
        auto filter = filter_builder << bsoncxx::builder::stream::finalize;
        
        auto cursor = coll.find(filter.view());
        if (cursor.begin() != cursor.end()) {
            auto document = *cursor.begin();
            auto idField = document["_id"];
            if (idField && idField.type() == bsoncxx::type::k_int32) {
                return idField.get_int32().value;
            }
        }
        return std::nullopt; 
    } catch (const std::exception& e) {
        std::cerr << "Error interacting with the database: " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::optional<uint32_t> DatabaseControls::idFind() {
    try {
        DatabaseHandler& db_json = DatabaseSingleton::getInstance();
        auto coll = db_json.getClient()[db_json.getDatabase()][db_json.getCollectionClient()];
        auto findOptions = mongocxx::options::find{};
        findOptions.limit(1);
        findOptions.sort(bsoncxx::builder::stream::document{} << "_id" << -1 << bsoncxx::builder::stream::finalize);

        auto cursor = coll.find({}, findOptions);

        if (cursor.begin() != cursor.end()) {
            auto document = *cursor.begin();

            auto idField = document["_id"];
            if (idField && idField.type() == bsoncxx::type::k_int32) {
                return idField.get_int32().value;
            }
        }

        return std::nullopt;
    } catch (const std::exception& e) {
        std::cerr << "Error interacting with the database: " << e.what() << std::endl;
        return std::nullopt;
    }
}
