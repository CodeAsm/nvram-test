ssh -o HostKeyAlgorithms=+ssh-rsa -o PubkeyAcceptedKeyTypes=+ssh-rsa user@192.168.2.32

/etc/apt/sources.list

deb http://archive.debian.org/debian/ lenny main contrib non-free
deb http://archive.debian.org/debian-security/ lenny/updates main contrib non-free

apt-get update
sudo apt-get install gcc make build-essential manpages-dev libc6-dev

then you can install

gcc nvramwrite.c -o nvramwrite -std=c99

https://unix.stackexchange.com/questions/66830/are-there-any-source-apt-repositories-for-debian-lenny