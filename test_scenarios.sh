#!/bin/bash
# Test script for suckless Battleship game

echo "=========================================="
echo "BATTLESHIP GAME TEST SUITE"
echo "=========================================="
echo ""

# Test 1: Valid ship placement
echo "Test 1: Valid ship placement"
cat > test1_input.txt << 'EOF'
A0
0
B0
0
C0
0
EOF

timeout 2 ./battleship < test1_input.txt > test1_output.txt 2>&1 || true
if grep -q "Placing your" test1_output.txt; then
    echo "✓ Test 1 PASSED: Ship placement prompts working"
else
    echo "✗ Test 1 FAILED: Ship placement prompts not found"
fi
echo ""

# Test 2: Invalid ship placement (out of bounds)
echo "Test 2: Invalid ship placement (out of bounds)"
cat > test2_input.txt << 'EOF'
E4
0
E3
0
A0
0
E4
1
E3
1
B0
0
E4
0
D0
0
C0
0
EOF

timeout 2 ./battleship < test2_input.txt > test2_output.txt 2>&1 || true
if grep -q "Error: Ship placement failed" test2_output.txt; then
    echo "✓ Test 2 PASSED: Out of bounds detection working"
else
    echo "✗ Test 2 FAILED: Out of bounds not detected properly"
fi
echo ""

# Test 3: Divine personality preserved
echo "Test 3: Divine Conquest personality"
cat > test3_input.txt << 'EOF'
A0
0
B0
0
C0
0
EOF

timeout 2 ./battleship < test3_input.txt > test3_output.txt 2>&1 || true
if grep -q "DIVINE CONQUEST" test3_output.txt && grep -q "holy" test3_output.txt; then
    echo "✓ Test 3 PASSED: Divine personality preserved"
else
    echo "✗ Test 3 FAILED: Divine personality missing"
fi
echo ""

# Test 4: Overlapping ship placement
echo "Test 4: Overlapping ship placement"
cat > test4_input.txt << 'EOF'
A0
0
A0
0
A1
0
B0
0
C0
0
EOF

timeout 2 ./battleship < test4_input.txt > test4_output.txt 2>&1 || true
if grep -q "Error: Ship placement failed" test4_output.txt; then
    echo "✓ Test 4 PASSED: Overlapping ships detected"
else
    echo "✗ Test 4 FAILED: Overlapping ships not detected"
fi
echo ""

# Cleanup
rm -f test*_input.txt test*_output.txt

echo "=========================================="
echo "TEST SUITE COMPLETE"
echo "=========================================="
