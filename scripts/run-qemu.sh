#!/usr/bin/env bash

set -e

KERNEL=$1/bzImage
IMAGE_FILE=$1/jarvis-dev-image-qemux86-64.ext4

command -v qemu-system-x86_64 > /dev/null
if [ $? != 0 ]; then
    echo -e "Error:"
    echo -e "QEMU is not installed"
    exit 1
fi

if [ -d "${KERNEL}" ]; then
    echo -e "Error:"
    echo -e "Kernel file <${KERNEL}> is absent"
    exit 1
fi

if [ -d "${IMAGE_FILE}" ]; then
    echo -e "Error:"
    echo -e "Image file <${IMAGE_FILE}> is absent"
    exit 1
fi

qemu-system-x86_64 \
  -enable-kvm \
  -kernel "${KERNEL}" \
  -cpu IvyBridge \
  -smp 2 \
  -m 2G \
  -vga std \
  -object rng-random,filename=/dev/urandom,id=rng0 -device virtio-rng-pci,rng=rng0 \
  -drive file="${IMAGE_FILE}",media=disk,format=raw \
  -audiodev pa,id=snd0,server="$XDG_RUNTIME_DIR/pulse/native" -device intel-hda -device hda-output,audiodev=snd0 \
  -netdev bridge,id=net0,br=virbr0 -device virtio-net-pci,netdev=net0,id=nic1 \
  -usb \
  -append "root=/dev/sda rw console=ttyS0 nokaslr" \
  -serial mon:stdio \
  -nographic
