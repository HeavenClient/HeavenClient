#!/bin/bash

set -ex

# Symlink all host-mounted nx files to the container
ln -s /root/nx/* /root/

exec "$@"