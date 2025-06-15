# 📦 Projet C : Logiciel Simplifié de Gestion de Versions (Type Git)

Ce projet en langage C est une implémentation simplifiée d’un système de gestion de versions à la manière de **Git**, développé dans le cadre du cours de **Programmation C Avancée** à l'**Institut International de Technologie - IIT Sfax**.

## 📌 Objectif

Créer un mini-système de gestion de versions en ligne de commande qui permet d’enregistrer un instantané (`snapshot`) d’un fichier donné, basé sur le **hachage SHA256** de son contenu.

---

## 🛠️ Fonctionnalités

- 📄 **Hashage SHA256** du contenu d’un fichier avec `sha256sum`
- 🧠 **Génération du chemin de stockage** basé sur le hash (`XX/rest_of_hash`)
- 💾 **Sauvegarde du fichier** dans une arborescence inspirée de Git (`blob`)
- 🗂️ **Liste chaînée personnalisée** pour manipuler des listes de chaînes
- 🧪 **Lecture/écriture de listes** dans des fichiers
- 🔍 **Vérification de fichiers existants**, lecture de répertoire (`dirent.h`)
- 📥 **Copie manuelle** d’un fichier ligne par ligne
- 🔧 Utilisation de la fonction `system` pour invoquer des commandes Bash (`mkdir`, `sha256sum`, etc.)

