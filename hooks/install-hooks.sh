#!/usr/bin/env bash
# install-hooks.sh — symlink the project hooks into .git/hooks.
#
# Usage (from the repository root):
#   bash hooks/install-hooks.sh

set -euo pipefail

REPO_ROOT=$(git rev-parse --show-toplevel)
HOOKS_SRC="$REPO_ROOT/hooks"
HOOKS_DEST="$REPO_ROOT/.git/hooks"

HOOKS=(pre-commit commit-msg)

for HOOK in "${HOOKS[@]}"; do
    SRC="$HOOKS_SRC/$HOOK"
    DEST="$HOOKS_DEST/$HOOK"

    if [[ ! -f "$SRC" ]]; then
        echo "⚠️  Hook source not found: $SRC — skipping."
        continue
    fi

    chmod +x "$SRC"

    if [[ -L "$DEST" ]]; then
        echo "🔗 Already linked: $HOOK"
    elif [[ -f "$DEST" ]]; then
        echo "⚠️  $HOOK already exists as a regular file in .git/hooks — skipping."
        echo "   Remove it manually and re-run this script if you want to use the project hook."
    else
        ln -s "$SRC" "$DEST"
        echo "✅ Installed: $HOOK"
    fi
done

echo ""
echo "Git hooks installed.  They will run automatically on the next commit."
