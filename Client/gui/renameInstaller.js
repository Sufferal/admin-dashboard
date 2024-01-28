const fs = require('fs');
const path = require('path');

const newInstallerName = 'ClientGUI';

const outputDir = path.join(__dirname, 'out', 'make', 'squirrel.windows', 'x64');

fs.readdirSync(outputDir).forEach((file) => {
    if (file.startsWith('ClientGUI-') && file.endsWith('.exe')) {
        const oldPath = path.join(outputDir, file);
        const newPath = path.join(outputDir, `${newInstallerName}.exe`);
        fs.renameSync(oldPath, newPath);
    }
});