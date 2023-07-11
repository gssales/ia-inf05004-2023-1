for problem in p*.pddl; do
    fast-downward domain.pddl $problem --search "astar(const())"

    # wait for enter
    read -p "Press enter to continue"
done