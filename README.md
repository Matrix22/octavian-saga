## Negru Mihai: Octavian's Saga

This readme presents the idea of solving the task for the octavian's saga project.

## **Table of Contents**

* [`trial`](#trial)
    * [`trial description`](#trial-description)
    * [`trial oracle interpretation`](#trial-oracle-interpretation)
    * [`trial time complexity`](#trial-time-complexity)
* [`rise`](#rise)
    * [`rise description`](#rise-description)
    * [`rise time complexity`](#rise-time-complexity)
* [`redemption`](#redemption)
    * [`redemption description`](#redemption-description)
    * [`redemption time complexity`](#redemption-time-complexity)
* [`feedback`](#feedback)
* [`links`](#links)

### `trial`

The basic idea is to reduce the Set Cover NP-complete problem, by reducing it to the SAT problem and then using a special utility to solve the SAT problem.

#### `trial description`

First we read the subsets and store the frequency of each element from the universe in witch subset it can be found, so the following representation would mean that:

```text
    1 -> { 1, 2, 3 }
    2 -> { 4 }
    3 -> { 3, 4, 5 }
```

that element number `1` can be found in the `1, 2` and `3` subset and so on.

Because we already have in the paperwork the SAT reduction for the Vertex Cover problem and knowing that `Vertex Cover <=P Set Cover`, I concluded that we can use the reduction and to adjust it in order to solve the Set Cover.

First in the Vertex Cover reduction we observe that just the last clauses describe the links between universe' elements and subsets, so the rest of the clauses depend on the universe's cardinal and the number of subsets that have to be chosen `AT MOST` to cover the whole universe.

>**Note:** Because the Vertex Cover differs from the Set Cover when talking about number of vertices/subsets chosen, the result may not be exact **K** subsets, but at most **K** subsets, so for this case we add another subsets until we reach the **K** subsets.

As described in the paperwork we do the following clauses:
* `K` clauses in order to select at most K subsets for the cover.
* `M` clauses in order that a subset to be chosen at most once or not at all.
* `K * M * (M - 1) / 2` clauses in order to select just one subset for the `first, second, ... , K` position in the cover of the universe. 
* `N` clauses in order to select such subsets that each element from the universe is covered at least once.

>**Note:** We think about the subsets as the *nodes* of the graph and the elements from the universe as the *edges* from the graph.

>*Note:* For `K = 1`, we should not write the `M` clauses that set that each subset is chosen at most once, because in the final boolean expression we can have the following representation `x ^ -x`, which evalates always as false, even though all the universe can be covered by only one subset.

#### `trial oracle interpretation`

If the boolean expression is satisfiable than we will get the following message

```text
    True
    <NR_OF_BOOL_VAR>
    BOOL_VAL_1 BOOL_VAL_2, ... BOOL_VAL_{M*K}
```

We can interpret the boolean variables as a matrix of `M` rows and `K` columns. So it means that if on
one row we find a positive value the subset **MUST** be chosen for the cover.

For example:

```text
    True
    6
    -1 2 3 -4 -5 -6

    // Can be interpreted as this

    True
    6
    -1 2
    3 -4
    -5 -6
```

Which means that in the final cover we should take the first and second subset to cover the universe.

>**Note:** This idea generelize for the `M` number of subsets and `K` number of subsets to cover the universe.

>**Note:** For some cases, because we used the Vertex Cover reduction we can have a small number of subsets chosen than the `K`. To solve the problem we just add some subsets in order to fill the `K` - covering subsets.

#### `trial time complexity`

The time complexity for the trial problem is found on creating the SAT boolean formula in order to be processed.

>**Note:** We do not take into consideration the time complexity for reading input, showing output and chechking if the boolean expression is satisifabile or not.

So the time complexity comes from:
* computiong `K` clauses
* computing `M` clauses
* computing `K * M * (M - 1) / 2`
* and computing `N` clauses

This means that:

```text
    T(N, M, K) = O(K + M + K * M (M - 1) / 2 + N) = O(N + K * M^2)
    
    // T(N, M, K) == time complexity
    // O == theta notation
```

### `rise`

This problem is basically solving the `trial` problem, however we should first create by ourselves the universe from the input data and mapping the stirng names to ints.

For this problem we will discuss just how I created the `universe` and how I mapped the strings into ints, because the rest of the problem is basically the trial problem, which means that the oracle interpreatation and time complexity are the same as the `trials's` one

#### `rise description`

First we read the cards that Octavian has in his collection, which should be removed from our generated `universe`.

Then we read the cards that Octavian **MUST** have in his cards deck.

From the cards that Octavian must have we remove the cards that Octavian already has, because we should not try to cover an element that is already covered by Octavian himself.

From the last statement, now we have created the whole `universe` (Big Bang isn't it?!)

After that we compute the elements frequency in each pack that Octavian can buy, so now we have made the mapping from the `rise` problem to the `trial` problem.

>**Note:** For this problem we set at first that at most `M` - number of packs can be chosen to cover the universe, however from the trial problem we know that we **ALWAYS** will get the optimal solution for covering the universe. That means that the problem is actualy not Set Cover but Vertex Cover, so we do not need to add extra packs in order to fill the Octavian's collection.

#### `rise time complexity`

As mentioned the rise time complexity is the same as the `trial's` one, however `K == M`

```text
    T(N, M) = O(M + M + M * M (M - 1) / 2 + N) = O(N + M^3)
    
    // T(N, M) == time complexity
    // O == theta notation
```

### `redemption`

The redemption problem tries to solve the `rise` problem, however not using the `oracles's` magic power and to use the power of `approximations`.

For solving this problem I used the greedy algorithm, because it is the best known algorithm that solves the Set Cover problem.

>**Note:** The greedy problem does not always give the optimal solution for the problem and I know that we can run some post-approximation algorithms in order to select the optimal solution, however this could bring a big overload on the time complexity for the problem.

One of the post-checks algorithm could be to take the covering subsets and to check if one or more subsets are covered by another subset.

For example:

```text
    ANS = { S1 = {1, 2, 3}, S2 = {4, 5}, S3 = {1, 2} }

    The S3 subset could be removed in order to have the optimal solution
    however this check would bring a big overload, because we have to check
    every pair of subsets {S_i, S_j} which is `len(ANS)^2 - len(ANS) (if we skip i == j)`
    and we would have to check if S_i includes in S_j or S_j includes in S_i
    which could be achieved with the following formula

    let S = S_i U S_j

    bool_ans S_i == S || S_j == S

    For constructuing the S we would need `|S_i| + |S_j|`, we do not substract the intersection of it
    because the subset S will be itereated to find that there already exists the same value.

    Comparing the two subsets also bring the complexity of `|S_i| + |S_j|`

    which overall is a big overload
```

#### `redemption description`

The greedy algorithm first creates an empty universe and every iteration tries to add a subset that will cover the most elements from the universe, if at one iteration no subset was found to cover another elements that means that the problem is non-satisfiable, when the empty universe becames the universe we stop the iterations.

For optimization purposes first we add in the empty universe the subset with the biggest number of elements, because this will cover the most elements from the universe and will remove some extra iterations.

After that we iterate over the left subsets and compute the maximum number of elements that can be covered by the subset, after finding it we add it's elements to the universe and remove the subset from the list of subsets.

#### `redemption time complexity`

According to Vazirani's book `Approximation Algorithms` the greedy algortihm is a `H_n` factor approximation algorithm

Also according to the T. H. Cormen's book `Introduction to Algorithms` the greedy algorithm is a `2-approximation` algorithm, which means that the greedy algorithm will give us a solution that is at most `2` times the optimal solution.

Analyzing the greedy algorithm we can see that we stop when the empty universe becomes the universe, so the worst case would be that each subset contains only one element, which would mean that we would have to iterate `N` times.

For eaxh iteration we iterate thorugh remaining subsets and compute the maximum number of elements that can be covered by the subset, which means that we have to iterat overall `M + (M - 1) + ... + 1` subsets, and computing the maximum number of elements that can be covered by the subset is `O(|S_i|)`.

So overall the time complexity for the greedy algorithm would be:

```text
    T(N, M) = O(N * (M + (M - 1) + ... + 1) * |S_max_size|) = O(N * M^2)
    
    // T(N, M) == time complexity
    // O == theta notation
```

### `feedback`

Overall I enjoyed solving this homework assignment, because I did not understood from courses where I can use the SAT reduction in real life problems (like buying some packs :))) ).

Also at the first glance I thought that this project was so hard that it was better not to do it at all, but after understanding what it has to be done it was pretty easy to solve and fun to do it.

Also if you had not given us the reduction for the `VertexCover <=P SAT`, I think it would be a hell to solve this assignment.

Also please, do not add links to some cards, I searched every link to find some usefull information and it was not so fun :))))) .

Overall I liked this homework.

### `links`

These are the links that made possible solving Octavian's Saga problem, with his collection of cards:
* https://cs.stackexchange.com/questions/121295/is-time-complexity-of-the-greedy-set-cover-algorithm-cubic
* https://www.amazon.com/Introduction-Algorithms-3rd-MIT-Press/dp/0262033844
* https://www.geeksforgeeks.org/greedy-approximate-algorithm-for-set-cover-problem/
* https://math.mit.edu/~goemans/18434S06/setcover-tamara.pdf
* https://en.wikipedia.org/wiki/Set_cover_problem
* https://cs.stackexchange.com/questions/76176/reduction-from-set-cover-to-vertex-cover





