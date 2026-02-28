#!/bin/bash

if command -v uv >/dev/null 2>&1; then
	uv venv cmakelang --python 3.11

	source cmakelang/bin/activate

	# Installation des outils
	uv pip install cmakelang isort yapf beautysh

	# Traitement des fichiers CMakeLists.txt
	echo "Fichiers CMakeLists.txt trouvés :"
	find ../ -name 'CMakeLists.txt' -type f -print
	find ../ -name 'CMakeLists.txt' -type f | xargs cmake-format -i

	# Traitement des fichiers Python (.py)
	echo "Fichiers Python (.py) trouvés :"
	find ../ -not -path '*/cmakelang/*' -name '*.py' -type f -print
	find ../ -not -path '*/cmakelang/*' -name '*.py' -type f | xargs isort
	find ../ -not -path '*/cmakelang/*' -name '*.py' -type f | xargs yapf -i

	# Traitement des fichiers shell (.sh)
	echo "Fichiers shell (.sh) trouvés :"
	find ../ -name '*.sh' -type f -print
	INDENT_SIZE=2
	find ../ -name '*.sh' -type f | xargs beautysh -i ${INDENT_SIZE} -t

	# Suppression de l'environnement virtuel
	rm -rf ./cmakelang
fi
