#!/bin/sh

# Vérification du nombre d'arguments
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 D:/"
    exit 1
fi

# Chemin du fichier source
source="./build/bbc-microbit-classic-gcc/source/microbit-samples-conbined.hex"

# Chemin du dossier de destination passé en argument
destination="$1"

# Copie du fichier
cp "$source" "$destination"

# Vérification de la copie
if [ $? -eq 0 ]; then
    echo "Déploiement réalisé avec succès."
else
    echo "Erreur lors du déploiement."
fi
