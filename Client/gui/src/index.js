const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const grpc = require(path.join(__dirname, '../node_modules', '@grpc', 'grpc-js'));
const protoLoader = require(path.join(__dirname, '../node_modules', '@grpc', 'proto-loader'));

const PROTO_PATH = path.join(__dirname, '../../gui_communication.proto');

const protoOptions = {
    enums: String
}

const appUpdateProto = grpc.loadPackageDefinition(protoLoader.loadSync(PROTO_PATH, protoOptions));

let mainWindow;

const createWindow = () => {
    mainWindow = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false,
            preload: path.join(__dirname, 'preload.js')
        }
    });

    const indexPath = path.join(__dirname, 'index.html');
    mainWindow.loadURL(indexPath);

    mainWindow.webContents.openDevTools();

    mainWindow.on('closed', () => {
        mainWindow = null;
    });
};

app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
        createWindow();
    }
});

app.on('ready', () => {
    createWindow();

    const client = new appUpdateProto.UpdateService('localhost:8080', grpc.credentials.createInsecure());

    ipcMain.on('fetch-data', async (event, args) => {
        fetchData(client, event);
    });
});

function fetchData(client, event) {
    client.getAppUpdate({}, (err, response) => {
        if (err) {
            console.error(err);
            event.reply('fetch-data-response', 'RPC failed');
        } else {
            console.log(response.updateResponse);
            event.reply('fetch-data-response', response.updateResponse);
        }
    });
}