#!/bin/bash

EXECUTABLE="QTMaat"

echo "Applying network permissions to $EXECUTABLE..."
sudo setcap cap_net_raw,cap_net_admin=eip "$EXECUTABLE"

echo "Done! Run your app with: ./build/QTMaat"
