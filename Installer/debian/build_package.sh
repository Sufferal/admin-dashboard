#!/bin/bash

fpm -s dir -t deb -n debian-installer -v 1.0.0 \
  --depends "npm, libgrpc-dev" \
  --maintainer "Ciprian ciprian.botnari@eset.com" \
  --description "Installer for the client application" \
  --architecture "amd64" \
  --after-install postinst.sh \
  --deb-user "$USER" \
  --deb-group "$(id -gn)" \
  ../../Client/build/backend/backend=/opt/eset/client/backend/ \
  ESET_backend.service=/opt/eset/client/service/ \
  ../../Shared=/opt/eset/ \
  ../../Client/gui=/opt/eset/client/ \
