# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  # The most common configuration options are documented and commented below.
  # For a complete reference, please see the online documentation at
  # https://docs.vagrantup.com.

  # Every Vagrant development environment requires a box. You can search for
  # boxes at https://vagrantcloud.com/search.
  config.vm.box = "hashicorp/bionic64"

  # Disable automatic box update checking. If you disable this, then
  # boxes will only be checked for updates when the user runs
  # `vagrant box outdated`. This is not recommended.
  # config.vm.box_check_update = false

  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine. In the example below,
  # accessing "localhost:8080" will access port 80 on the guest machine.
  # NOTE: This will enable public access to the opened port
  # config.vm.network "forwarded_port", guest: 80, host: 8080

  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine and only allow access
  # via 127.0.0.1 to disable public access
  # config.vm.network "forwarded_port", guest: 5001, host: 5000, host_ip: "127.0.0.1"

  # Create a private network, which allows host-only access to the machine
  # using a specific IP.
  # config.vm.network "private_network", ip: "192.168.33.10"

  # Create a public network, which generally matched to bridged network.
  # Bridged networks make the machine appear as another physical device on
  # your network.
  # config.vm.network "public_network"

  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
  # Example for VirtualBox:
  config.vm.provider "virtualbox" do |vb|
    # Display the VirtualBox GUI when booting the machine
    vb.gui = true
  
    # Customize the amount of memory on the VM:
    vb.memory = "12228"
    vb.cpus = 4
  end

  # Share an additional folder to the guest VM. The first argument is
  # the path on the host to the actual folder. The second argument is
  # the path on the guest to mount the folder. And the optional third
  # argument is a set of non-required options.
  config.vm.synced_folder "./", "/home/vagrant/Desktop/HeavenClient"

  config.vm.provision :shell, :inline => $BOOTSTRAP_SCRIPT # see below

end

$BOOTSTRAP_SCRIPT = <<EOF
  export DEBIAN_FRONTEND=noninteractive
  set -ex
  sudo apt-get update


  # ---------- Setup GUI based VM ----------
  # https://askubuntu.com/questions/1067929/on-18-04-package-virtualbox-guest-utils-does-not-exist
  sudo apt-add-repository multiverse
  sudo apt-get update

  # Install xfce and virtualbox additions
  # # (Not sure if these packages could be helpful as well: virtualbox-guest-utils-hwe virtualbox-guest-x11-hwe)
  sudo apt-get install -y xfce4 virtualbox-guest-dkms virtualbox-guest-utils virtualbox-guest-x11

  # Permit anyone to start the GUI
  sudo sed -i 's/allowed_users=.*$/allowed_users=anybody/' /etc/X11/Xwrapper.config

  # Optional: Use LightDM login screen (-> not required to run "startx")
  sudo apt-get install -y lightdm lightdm-gtk-greeter

  # Optional: Install a more feature-rich applications menu
  sudo apt-get install -y xfce4-whiskermenu-plugin


  # ---------- Compilation of HeavenClient and its dependencies ----------
  # Add repo for latest cmake
  wget -qO - https://apt.kitware.com/keys/kitware-archive-latest.asc | sudo apt-key add -
  sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'
  sudo apt-get update

  # Install build tools
  sudo apt-get install -y \
    apt-transport-https \
    ca-certificates \
    gnupg \
    software-properties-common \
    build-essential \
    cmake \
    autoconf \
    libtool \
    pkg-config \
    git

  # Install build dependencies (required for HeavenClient and HeavenClient's dependencies)
  sudo apt-get install -y \
    libopenal-dev libvorbis-dev libopusfile-dev libsndfile1-dev \
    xorg-dev \
    libgl1-mesa-dev


  # Initiate HeaventClient compilation
  cd /home/vagrant/Desktop/HeavenClient

  # Build HeavenClient dependencies
  ./build-deps.sh

  # Build HeavenClient
  CORES=4
  rm -rf build && \
  mkdir build && \
  cd build && \
  cmake .. && \
  make -j$CORES

  # Set ENV vars
  echo "export DISPLAY=:0" >> /home/vagrant/.profile


  # ---------- VM auto reboot ----------
  # Reboot server to setup installed desktop environment
  if [ ! -f ~/runonce ]
  then
    sudo reboot
    touch ~/runonce
  fi

EOF
