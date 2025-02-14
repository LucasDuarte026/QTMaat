#!/bin/bash
EXECUTABLE="QTMaat"

echo "Applying permissions to $EXECUTABLE..."

# Permissões para acesso a redes e dispositivos USB
sudo setcap cap_net_raw,cap_net_admin,cap_dac_override,cap_sys_rawio=eip "$EXECUTABLE"

echo "Done! Now run your app without sudo"
