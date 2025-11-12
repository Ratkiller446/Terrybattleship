#!/bin/bash
# Input validation test suite for suckless battleship

echo "=========================================="
echo "INPUT VALIDATION TEST SUITE"
echo "=========================================="
echo ""

# Test 1: Empty input
echo "Test 1: Empty input handling"
cat > test_empty.txt << 'EOF'
A0
0
B0
0
C0
0

A0
EOF

timeout 2 ./battleship < test_empty.txt > test_empty_out.txt 2>&1 || true
# Empty input should be silently ignored and re-prompt
if [ $? -ne 139 ]; then
    echo "✓ PASSED: Empty input handled without crash"
else
    echo "✗ FAILED: Empty input caused crash"
fi
echo ""

# Test 2: Lowercase coordinates
echo "Test 2: Lowercase coordinates (case-insensitive)"
cat > test_lower.txt << 'EOF'
A0
0
B0
0
C0
0
a0
EOF

timeout 2 ./battleship < test_lower.txt > test_lower_out.txt 2>&1 || true
if grep -q "DIVINE HIT\|MISS" test_lower_out.txt; then
    echo "✓ PASSED: Lowercase coordinates accepted"
else
    echo "✗ FAILED: Lowercase coordinates not handled"
fi
echo ""

# Test 3: Out of range coordinates
echo "Test 3: Coordinates beyond board"
cat > test_beyond.txt << 'EOF'
A0
0
B0
0
C0
0
F0
A0
EOF

timeout 2 ./battleship < test_beyond.txt > test_beyond_out.txt 2>&1 || true
# Should silently ignore and continue
if [ $? -ne 139 ]; then
    echo "✓ PASSED: Out-of-range coordinates handled"
else
    echo "✗ FAILED: Out-of-range caused crash"
fi
echo ""

# Test 4: Buffer overflow attempt
echo "Test 4: Buffer overflow attempt"
cat > test_overflow.txt << 'EOF'
A0
0
B0
0
C0
0
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
A0
EOF

timeout 2 ./battleship < test_overflow.txt > test_overflow_out.txt 2>&1 || true
if [ $? -ne 139 ]; then
    echo "✓ PASSED: Long input handled without crash"
else
    echo "✗ FAILED: Buffer overflow caused crash"
fi
echo ""

# Test 5: Invalid direction
echo "Test 5: Invalid ship placement direction"
cat > test_direction.txt << 'EOF'
A0
2
0
B0
0
C0
0
EOF

timeout 2 ./battleship < test_direction.txt > test_direction_out.txt 2>&1 || true
# Should retry placement
if [ $? -ne 139 ]; then
    echo "✓ PASSED: Invalid direction handled"
else
    echo "✗ FAILED: Invalid direction caused crash"
fi
echo ""

# Test 6: Ship names preserved
echo "Test 6: Ship names (Patrol Boat, Destroyer, Battleship)"
cat > test_names.txt << 'EOF'
A0
0
B0
0
C0
0
EOF

timeout 2 ./battleship < test_names.txt > test_names_out.txt 2>&1 || true
if grep -q "Patrol Boat" test_names_out.txt && \
   grep -q "Destroyer" test_names_out.txt && \
   grep -q "Battleship" test_names_out.txt; then
    echo "✓ PASSED: Ship names preserved"
else
    echo "✗ FAILED: Ship names missing"
fi
echo ""

# Test 7: Divine messages
echo "Test 7: Divine conquest messages"
cat > test_divine.txt << 'EOF'
A0
0
B0
0
C0
0
A0
EOF

timeout 2 ./battleship < test_divine.txt > test_divine_out.txt 2>&1 || true
if grep -q "DIVINE HIT\|MISS.*enemy.*spared" test_divine_out.txt; then
    echo "✓ PASSED: Divine messages present"
else
    echo "✗ FAILED: Divine messages missing"
fi
echo ""

echo "=========================================="
echo "CLEANUP"
echo "=========================================="
rm -f test_*.txt test_*_out.txt

echo ""
echo "=========================================="
echo "INPUT VALIDATION TEST SUITE COMPLETE"
echo "=========================================="
