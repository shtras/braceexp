#!/bin/bash
function run_test()
{
	diff <(echo "$1" | ../bin/braceexp) <(echo ${@:2})
	if [[ $? != 0 ]] ; then
		echo "Results mismatch with $1"
		exit 1
	fi
}

run_test "A{B,C}" A{B,C}
run_test "A{B,C}" A{B,C}
run_test "{A,B,C}" {A,B,C}
run_test "{A,B}{C,D}" {A,B}{C,D}
run_test "{A,B{C,D}}" {A,B{C,D}}
run_test "{ABC}" ABC # Requirement differs from bash behavior
run_test "ABC" ABC
run_test "A{B,C}" A{B,C}
run_test "{{{{{Z,Y}W,V}U,T}S,R}Q,P}{A,B{C,D{E,F{G,H{I,J{K,L{M,N{O,P{Q,R{S,T{U,V{W,X{Y,Z}}}}}}}}}}}}}" {{{{{Z,Y}W,V}U,T}S,R}Q,P}{A,B{C,D{E,F{G,H{I,J{K,L{M,N{O,P{Q,R{S,T{U,V{W,X{Y,Z}}}}}}}}}}}}}
run_test "{A,B}C{D,E}" {A,B}C{D,E}
run_test "{AB,CD}EF{GH,JK}" {AB,CD}EF{GH,JK}
run_test "{A,B}{C,D}{E,F}{G,H}" {A,B}{C,D}{E,F}{G,H}

run_test "}ABC"
run_test "{ABC"
run_test "}{"
run_test "{}"
run_test "A,B,C"
run_test "{A,}"
run_test "A B"