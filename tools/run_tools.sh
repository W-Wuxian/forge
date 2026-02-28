#!/bin/bash

# run_tools.sh - Lance les outils de formatage (1, 2, N outils ou TOUS)
# Usage: ./run_tools.sh [tool_clang_format.sh] [tool_format_other.sh] [tool_header_guard.py]
# Par défaut: lance TOUS les outils (SAUF run_tools.sh lui-même)

set -euo pipefail

TOOLS_DIR="$(dirname "$0")"
SELF_SCRIPT="$(basename "$0")"

TOOLS=(
	"tool_clang_format.sh"
	"tool_format_other.sh"
	"tool_header_guard.py"
)

# Fonction pour lancer un outil
run_tool() {
	local tool_name="$1"

	# ❌ EXCEPTION : ne lance PAS run_tools.sh lui-même
	if [ "$tool_name" = "$SELF_SCRIPT" ]; then
		echo "⏭️  Skip $tool_name (script principal)"
		return
	fi

	echo "📦 Lancement de $tool_name..."
	if [ -f "$TOOLS_DIR/$tool_name" ]; then
		"$TOOLS_DIR/$tool_name"
		echo "✅ $tool_name terminé"
	else
		echo "❌ Erreur: $tool_name introuvable dans $TOOLS_DIR"
		exit 1
	fi
}

# Aucun argument → TOUS les outils (sauf lui-même)
if [ $# -eq 0 ]; then
	echo "🚀 Lancement de TOUS les outils de formatage (sauf $SELF_SCRIPT)..."
	for tool in "${TOOLS[@]}"; do
		run_tool "$tool"
	done
else
	# 1, 2, N outils spécifiés (skip si run_tools.sh demandé)
	echo "🚀 Lancement des outils demandés ($# outil(s))..."
	for tool_name in "$@"; do
		run_tool "$tool_name"
	done
fi

echo "🎉 TOUS les outils terminés avec succès !"