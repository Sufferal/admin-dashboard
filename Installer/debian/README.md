# Debian Installer

## Dependencies
* `fpm`
* `npm`
* `grpc`


## Build and install the Debian package
1. Open the terminal inside `Installer/debian` folder
2. Run the following command to build the Debian package
```
./build_package.sh
```

3. Install the Debian package
```
sudo dpkg -i Installer/debian/build/<package_name>.deb
```

## Run the application
1. Open the terminal inside `/opt/eset/client/gui` folder
2. Run the script to start the application
```
./run_app.sh
```




