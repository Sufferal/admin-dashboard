#!/bin/bash

# Set the variable for the backend executable path
BACKEND_EXECUTABLE_PATH="/opt/eset/client/backend/backend"

# Check if the backend executable file exists
if [ ! -f "$BACKEND_EXECUTABLE_PATH" ]; then
  echo "Backend executable not found: $BACKEND_EXECUTABLE_PATH"
  exit 1
fi

echo 'Registering backend service...'
chmod +x /opt/eset/client/backend/backend

BACKEND_SERVICE_PATH="/opt/eset/client/service/ESET_backend.service"
# Check if the backend service exists
if [ ! -f "$BACKEND_SERVICE_PATH" ]; then
  echo "ESET_backend service not found: $BACKEND_SERVICE_PATH"
  exit 1
fi

# If the service is already running then stop, disable and remove it
if systemctl is-active --quiet ESET_backend.service; then
  echo 'Stopping ESET_backend.service...'
  systemctl stop ESET_backend.service

  echo 'Disabling ESET_backend.service...'
  systemctl disable ESET_backend.service

  echo 'Removing ESET_backend.service from /etc/systemd/system/...'
  rm /etc/systemd/system/ESET_backend.service
fi

# Copy the service configuration file to the appropriate location
cp "$BACKEND_SERVICE_PATH" /etc/systemd/system/

systemctl daemon-reload
systemctl enable ESET_backend.service
systemctl start ESET_backend.service
