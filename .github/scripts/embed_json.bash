#!/usr/bin/env bash

INPUT="$1"
OUTPUT="$2"

if [[ -z "$INPUT" || -z "$OUTPUT" ]]; then
    echo "Usage: $0 <input.json> <output.hh>"
    exit 1
fi

cat > "$OUTPUT" <<EOF
#pragma once
#include <string>

inline const std::string DEFAULT_CONFIG_JSON_STRING {
R"(
EOF

cat "$INPUT" >> "$OUTPUT"

cat >> "$OUTPUT" <<EOF
)"
};
EOF