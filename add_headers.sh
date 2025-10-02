#!/bin/bash

# Script to add minishell.h header to all parsing files

echo "Adding headers to parsing files..."

# Function to add header if not present
add_header() {
    local file=$1
    if [ -f "$file" ]; then
        # Check if header already there
        if ! grep -q '#include.*minishell.h' "$file" 2>/dev/null; then
            echo "Processing: $file"
            # Create backup
            cp "$file" "${file}.bak"
            # Add header
            {
                echo '#include "../includes/minishell.h"'
                echo ''
                cat "${file}.bak"
            } > "$file"
            rm "${file}.bak"
            echo "  ✓ Added header"
        else
            echo "  - Already has header: $file"
        fi
    fi
}

# Process all parsing files
for file in parsing/*.c; do
    add_header "$file"
done

# Process var_expansion files
for file in var_expansion/*.c; do
    add_header "$file"
done

# Process wild_card files
for file in wild_card/*.c; do
    add_header "$file"
done

# Process error_checks files
for file in error_checks/*.c; do
    add_header "$file"
done

# Process utils/helper_funcs files
for file in utils/helper_funcs/*.c; do
    add_header "$file"
done

echo ""
echo "Done! All files processed."
echo ""
echo "Next steps:"
echo "1. Run: make"
echo "2. Fix any compilation errors (especially get_env_value conflict)"
echo "3. Run: ./minishell"
