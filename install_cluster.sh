#!/bin/bash

echo "Instalando libs"
apt-get install libopenmpi-dev openmpi-bin git

echo "Criando usuários"
adduser ompi

cd /home/ompi

echo "Baixando código"
git clone https://github.com/GustavoKatel/ompie.git

echo "Autorizando via ssh"
mkdir .ssh

echo "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC7nMjV58F/WjT/43QstSptaw/H9nG+1naWz+MNvWAsYL2TilBg+MS0kiI3g2lMwQaULZhbxGRIRD+5sgUrrs8JUmBsPvA9UXcWgwnwrf/IfUOVKlz+iu882sxFVCeJC2sQXf9iNoR5VMs3VH8c+TD4cdb7Szljk2DqGp+Y4iacDQVL8+ZCCkumFfpQeQAjuYNHZX+LvoVL68Z5oic1iGsunFT1uMqGTVX+7jlfs+rk8dofgYowRnEN9sRy0tpduop9vmfv1tCG3EKjfD4xZdfIasIvi5AbNKFj7ytoHncnofmT0jHXkfjIR7cltlUaOzGoZS/yHAFS1xTUhdlTlZJ9 ompi@deathstar" > .ssh/authorized_keys

chmod 644 .ssh/authorized_keys

