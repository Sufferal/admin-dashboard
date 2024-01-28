#!/bin/bash

start_gui() {
  nohup npm start &
  cd ..
}

# Check the status of ESET_backend.service
if systemctl is-active --quiet ESET_backend.service; then
  echo "ESET_backend.service is active. Starting GUI..."
  start_gui
else
  echo "ESET_backend.service is not active. GUI will not be started."
fi