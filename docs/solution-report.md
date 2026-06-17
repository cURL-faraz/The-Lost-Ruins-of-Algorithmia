### Problem 1: "The Time-Limited Expedition" 

**Problem Overview**

 We are given an $R \times C$ grid in which certain cells are passable and others are blocked. A distinguished start cell $S$ and end cell $E$ are given, together with a set of *treasure* cells and a strict bound $M$ on the total number of moves (steps to orthogonally adjacent passable cells). The objective is to determine the **maximum number of treasures** that can be collected along *some* walk from $S$ to $E$ whose total length does not exceed $M$ moves. A treasure is collected the first time its cell is visited, and each treasure counts at most once.

 **Modeling the Grid as a Graph** 

We view the grid as an undirected, unweighted graph: every passable cell is a vertex, and an edge joins two passable cells that are orthogonally adjacent. With $|V| = R\cdot C$ vertices, the number of edges is bounded by $|E| = O\big(R(C-1) + C(R-1)\big) = O(R\cdot C)$, since each cell can connect to at most four neighbors. On such a graph the length of the shortest walk between two cells equals their shortest-path distance, and because all edges have unit weight this distance is computed exactly by **breadth-first search**. 

**The Central Idea: Labeling Paths by Points of Interest** 

The decisive observation is that, although a walk visits many ordinary cells, the only cells that *matter* to the objective are the start $S$, the end $E$, and the treasures. Any treasure-collecting walk can therefore be **labeled by the sequence of treasures it collects, in the order they are first reached**, producing a label of the form $$S \;\to\; T_{i_1} \;\to\; T_{i_2} \;\to\; \cdots \;\to\; T_{i_k} \;\to\; E.$$ Because every treasure is collected at most once, this label is a *permutation of a subset* of the treasures, framed by the fixed endpoints $S$ and $E$. Crucially, once such a label is fixed, the **cheapest realization** of it is obtained by connecting consecutive points of interest by their individual shortest paths: the minimum number of moves needed to realize the label is the sum of shortest-path distances $S\to T_{i_1}$, $T_{i_1}\to T_{i_2}$, …, $T_{i_{k-1}}\to T_{i_k}$, and finally $T_{i_k}\to E$. This reduces the entire continuous routing problem to a question purely about the *order in which a chosen subset of treasures is visited*. 

**Distance Precomputation** 

To evaluate any label we need only the shortest-path distances *between points of interest*: from $S$ to each treasure, between every pair of treasures, and from each treasure to $E$. These are obtained by running one BFS from $S$ and one BFS from each treasure; a single BFS from a source yields its distance to *all* cells, so the treasure-to-treasure distances and the treasure-to-$E$ distances are all read off directly. Letting $T$ denote the number of treasures, this preprocessing performs $O(T)$ breadth-first searches, each costing $O(V+E)=O(R\cdot C)$, for a total of $O(T\cdot R\cdot C)$. 

**Dynamic Programming over Subsets and Orders**

 A naive enumeration of all labels would consider every ordering of every subset of treasures — super-exponential. The key efficiency comes from recognizing **overlapping subproblems**: the minimum cost to collect a particular *set* of treasures and arrive at a particular *last* treasure does not depend on the order in which the earlier treasures were collected — only on which set has been collected and where we currently stand. This is exactly the structure that dynamic programming exploits. We therefore define the table $$dp[\,\mathrm{TS}\,][\,LT\,] = \text{the minimum number of moves of any walk that starts at } S,\ \text{collects precisely the treasure set } \mathrm{TS},\ \text{and ends at treasure } LT \in \mathrm{TS},$$ where $\mathrm{TS}$ is represented compactly as a **bitmask** over the $T$ treasures (bit $j$ set means treasure $j$ has been collected) and $LT$ is the most recently collected treasure. The table is filled in increasing order of the bitmask integer value, using the recurrence $$dp[\mathrm{TS}][LT] \;=\; \min_{\,T' \in\, \mathrm{TS}\setminus\{LT\}} \Big( dp[\,\mathrm{TS}\setminus\{LT\}\,][T'] \;+\; \text{dist}(T', LT)\Big),$$ with the base case $dp[\{LT\}][LT] = \text{dist}(S, LT)$ for each single treasure $LT$. In words: to be at $LT$ having collected $\mathrm{TS}$, we must previously have been at *some other* treasure $T'$ in $\mathrm{TS}$ having collected everything except $LT$, then walked the shortest path from $T'$ to $LT$; we take the best such predecessor. 

**Extracting the Answer** 

A label is *feasible* only if its full realization, including the closing leg back to the exit, fits within the budget. Hence whenever $dp[\mathrm{TS}][LT]$ is finalized, we test $$dp[\mathrm{TS}][LT] + \text{dist}(LT, E) \;\le\; M,$$ and if it holds, the entire set $\mathrm{TS}$ is collectible within budget; we update the running maximum with the number of treasures in $\mathrm{TS}$, i.e. the population count of the bitmask: $$\text{max\_T} \;\gets\; \max\big(\text{max\_T},\ |\mathrm{TS}|\big).$$ Taking the maximum over all states yields the answer. Subsets whose distance already exceeds $M$, and treasures unreachable from $S$, contribute nothing and are naturally excluded because their distances are effectively infinite, so they can never satisfy the feasibility test. 

**Why the Decomposition Is Valid** 

The correctness rests on the *optimal-substructure* property: the cheapest way to collect a set $\mathrm{TS}$ ending at $LT$ necessarily contains, as a prefix, a cheapest way to collect $\text{mask}\setminus\{LT\}$ ending at some $T$. If the optimal sequence exists, this DP *must* find it because it exhausts all sequences of distinct points of interest efficiently.

**Complexity Analysis**

- **Time Complexity:** $O(T\cdot RC + 2^T \cdot T^2)$.
  - Precomputing distances from the source and from up to $T$ treasures via BFS costs $(T+1)\cdot O(RC) = O(T\cdot RC)$.
  - The DP table has $2^T \cdot T$ states. For each state, we iterate over up to $T$ potential predecessor treasures to solve the recurrence, costing $O(T)$ per state. The total transition cost is thus $O(2^T \cdot T^2)$.
- **Memory Complexity:** $O(T \cdot RC + 2^T \cdot T)$.
  - Storing the grid and the distance buffers from the source and all treasures to all other vertices takes $O(T \cdot RC)$.
  - Storing the DP table takes $O(2^T \cdot T)$.

### Problem 2: "Mapping the Ruins: Solution Report"

**Problem Overview**

We are given an `R x C` grid where each cell is either passable or
blocked. The cartographer wants a single global statistic: the
**average shortest travel time** between every pair of distinct,
mutually reachable chambers. If no such pair exists, the answer is
`0.00`.

**Modeling the Grid as a Graph**

Exactly as in the previous problem, we treat the ruin as an
**unweighted, undirected graph** $G = (V, E)$:

- **Vertices** $V$: every passable cell.
- **Edges** $E$: an edge joins two passable cells that are
  **orthogonally adjacent** (up, down, left, right). Movement is
  strictly 4-directional, and every move costs exactly $1$.

Because all edges have unit cost, the shortest distance between two
cells is simply the number of edges on a shortest path, which is
precisely what **Breadth-First Search (BFS)** computes layer by layer.
This is why BFS — not a weighted shortest-path algorithm — is the
correct and minimal tool here.

**Central Idea: BFS From Every Source**

The quantity we need is the average of $d(u, v)$ over all ordered pairs
of distinct, reachable vertices:
$$
\text{average} =
\frac{\displaystyle\sum_{\substack{(u,v):\, u \neq v \\ d(u,v) < \infty}} d(u,v)}
     {\bigl|\{(u,v) : u \neq v,\; d(u,v) < \infty\}\bigr|}
$$

A single BFS from a source $s$ yields $d(s, \cdot)$ to **all** vertices
reachable from $s$ in one sweep. So the strategy is direct:

Run one BFS using **each passable vertex** as the source. Each run
produces one full row of pairwise distances. For every source we examine all other vertices and accept a distance into the running totals only when it is:

- **not $0$**, which excludes the source talking to itself
  (enforcing $u \neq v$); and
- **not the "infinity" sentinel**, which excludes vertices lying in a
  different connected component (genuinely unreachable pairs).

Two accumulators are maintained across all BFS runs: the **sum** of all
accepted distances and their **count**. The final answer is the sum
divided by the count.

**Complexity Analysis**

Let $V = R \cdot C$ be the number of cells.

**Time.** A single BFS runs in
$O(V + E)$. On a 4-directional grid the edge count is
$$
E = O\bigl(R(C-1) + C(R-1)\bigr) = O(R \cdot C),
$$

so each BFS costs $O(R \cdot C)$. We repeat this from every passable
vertex, i.e. $O(V) = O(R \cdot C)$ times. The total time is therefore
$$
O\bigl(V(V + E)\bigr) = O\bigl((R \cdot C)^2\bigr).
$$

**Memory.** The adjacency list is built once and stores
$O(E) = O(R \cdot C)$ entries. Each BFS needs a distance array of size
exactly $R \cdot C$, and this buffer is reused across sources rather
than stored per source, so we never materialize a full $V \times V$
matrix. The total memory is
$$
O(R \cdot C).
$$


### Problem 3: "The Minimal Expedition"

**Problem Overview**

We are given an `R x C` grid with a start `S`, an exit `E`, walls `#`,
and a set of treasures. The previous expedition asked how many treasures
we could gather within a fixed move budget. This problem **inverts the
objective**: we are told a target $K$ and must find the **minimum number
of moves** required to walk from `S` to `E` while collecting **at least
$K$ treasures**. If no such walk exists, the expedition is impossible.

**Modeling the Grid as a Graph**

As before, we treat the ruin as an **unweighted, undirected graph**
$G = (V, E)$:

- **Vertices** $V$: every passable cell.
- **Edges** $E$: orthogonal adjacency between passable cells, each move
  costing exactly $1$.

Because all moves cost one, the shortest distance between any two cells
is the BFS layer distance. This is the only shortest-path tool we need.

**The Path-Labeling Viewpoint**

The key idea, carried over from Problem 1, is to stop thinking about
individual grid cells and instead think about the **order in which
treasures are visited**. Any valid expedition is a walk of the form
$$
S \;\to\; T_{i_1} \;\to\; T_{i_2} \;\to\; \cdots \;\to\; T_{i_m} \;\to\; E,
$$

where $\{T_{i_1}, \dots, T_{i_m}\}$ is the set of collected treasures and
$m \ge K$. The total cost of such a walk is the sum of shortest
distances between consecutive labeled points:

$$
d(S, T_{i_1}) + \sum_{j=1}^{m-1} d(T_{i_j}, T_{i_{j+1}}) + d(T_{i_m}, E).
$$

Each segment is realized by an actual shortest grid walk, and
concatenating those segments yields a valid walk whose length equals the
sum of those distances. This reduces the geometry of the maze to a small
metric over the points of interest $\{S, T_0, \dots, T_{T-1}, E\}$.

**Preprocessing: BFS From Each Point of Interest**

We run one BFS from `S` and one from **each reachable treasure**,
recording the full distance row each time. This gives every pairwise
distance among $S$, the treasures, and $E$ that the DP will need. Index
$0$ stores distances from $S$; indices $1 \dots T$ store distances from
the treasures.

**The Bitmask DP**

We reuse the subset DP from Problem 1, changing only what we ask of it.

**State.**
$$
dp[\text{mask}][\text{last}] = \text{minimum moves to start at } S,
\text{ collect exactly the treasures in mask, and end on treasure last.}
$$

**Base case.** For a single treasure $t$,
$$
dp[\{t\}][t] = d(S, T_t).
$$

**Recurrence.** Extending a subset by appending a new last treasure:
$$
dp[\text{mask}][\text{last}] =
\min_{\substack{\text{prev} \in \text{mask}\setminus\{\text{last}\}}}
\Bigl( dp[\text{mask}\setminus\{\text{last}\}][\text{prev}]
       + d(T_{\text{prev}}, T_{\text{last}}) \Bigr).
$$

The table is filled in increasing **integer value of the mask**, which
guarantees every subset is computed before any superset that depends on
it.

**Extracting the Minimum Budget**

The objective change lives entirely in the extraction step. Each time a
state `dp[mask][last]` is finalized, we check whether the subset already
satisfies the target:
$$
\text{if } \operatorname{popcount}(\text{mask}) \ge K: \quad
\text{min\_M} \leftarrow
\min\bigl(\text{min\_M},\; dp[\text{mask}][\text{last}] + d(T_{\text{last}}, E)\bigr).
$$

Because **every** subset of size $\ge K$ is examined, this correctly
minimizes over "at least $K$ treasures," not merely exactly $K$. If
`min_M` is never lowered from its sentinel value, no path collects $K$
treasures and the task is reported impossible.

**Complexity Analysis**

Let $T$ be the number of treasures and $V = R \cdot C$.

**Time.** Preprocessing performs $O(T)$ BFS runs at $O(R \cdot C)$ each,
i.e. $O(T \cdot R \cdot C)$. The DP visits $2^T$ masks, each with $T$
choices of `last` and $T$ predecessors, i.e. $O(2^T \cdot T^2)$.
The extraction step is absorbed into the DP loop. Total:

$$
O\bigl(2^T \cdot T^2 + T \cdot R \cdot C\bigr).
$$

**Memory.** The DP table holds $O(2^T \cdot T)$ entries, and the
distance rows hold $O(T \cdot R \cdot C)$. Total:

$$
O\bigl(2^T \cdot T + T \cdot R \cdot C\bigr).
$$

### Problem 4: "The Rescue Mission"

**Problem Overview **

Several rescue teams begin at chamber $S$ and must all travel to chamber
$E$. The corridors are so narrow that **no two teams may ever stand in the
same chamber at the same time** — the only chambers that may be shared are
the start $S$ and the destination $E$ themselves. We must determine the
**largest number of teams** that can make this journey at once. Because two teams may never meet at any intermediate chamber, the route of
each team must avoid every chamber used by every other team. In graph
language, we are looking for the **maximum number of paths from $S$ to $E$
that share no vertex** other than the two endpoints.

**Modelling the Cave as a Directed Graph**

We turn the grid into a directed graph. The key difficulty is that the
restriction is placed on **chambers (vertices)**, whereas the natural
quantity a flow algorithm controls is the amount of traffic on
**connections (edges)**. We bridge this gap with a classic device called
**vertex splitting**.

**Splitting Each Cell into Two**

Every passable cell is represented not by one vertex but by two:

- an **in-vertex** $v_{in}$, which receives all traffic arriving at the
  chamber, and
- an **out-vertex** $v_{out}$, which sends all traffic leaving the
  chamber.

These two are joined by a single internal connection
$v_{in} \to v_{out}$. The whole point of this internal connection is that
**all traffic passing through a chamber is forced to cross it**, so by
limiting *its* capacity we limit how many paths may use the chamber.

**Capacities That Encode the Rules**

We assign the capacities as follows:

- For an ordinary chamber $v$ (not an endpoint), the internal connection
  $v_{in} \to v_{out}$ has capacity $1$. This is the heart of the model:
  it guarantees that **at most one team can ever pass through that
  chamber**, which is exactly the no-sharing rule.

- For the endpoints, the internal connections $S_{in} \to S_{out}$ and
  $E_{in} \to E_{out}$ are left **uncapped** (effectively infinite),
  because all teams are explicitly permitted to share $S$ and $E$.

**Connecting Neighbouring Chambers**

Whenever two chambers $u$ and $v$ are adjacent in the grid, a team may
walk from one to the other. We express this by adding, for the cell $u$
being processed, the connection
$$
u_{out} \to v_{in},
$$

left **uncapped**. We deliberately put no limit here: a team leaving the
*out* side of $u$ flows into the *in* side of $v$, and is then immediately
subjected to the capacity-$1$ internal connection of $v$. So the moving
constraint never needs to live on the corridor itself — it is already
enforced by the chambers at both ends.

**Where the Flow Begins and Ends**

We compute flow from $S_{out}$ as the **source** to $E_{in}$ as the
**sink**. Choosing these specific halves of the endpoints is intentional:
it leaves the uncapped internal connections of $S$ and $E$ entirely
*outside* the path the flow travels, so the shared endpoints can never
become a bottleneck. With this arrangement, **the value of the maximum
flow is exactly the maximum number of teams** that can reach $E$ from $S$
without sharing a chamber.

**The Algorithm: Finding the Maximum Flow**

Having built the network, we find the maximum flow from $S_{out}$ to
$E_{in}$. We do this with **Dinic's algorithm**, which works in repeated
rounds. Each round has two stages.

**Stage One — A Breadth-First Search to Build Levels**

We run a breadth-first search starting from the source. It assigns every
chamber a **level**: its distance, measured in number of connections, from
the source — but it only walks along connections that still have spare
capacity. This produces a *layered* picture of the graph, in which every
useful connection steps from one level to exactly the next.

The breadth-first search serves a second purpose: it tells us whether the
sink $E_{in}$ is still reachable from the source at all. As the algorithm
pushes flow and uses up capacities, the sink may eventually become
unreachable; when that happens, no more teams can be sent and the
algorithm stops. A single breadth-first search costs $O(V + E)$.

**Stage Two — A Depth-First Search to Push Flow**

Within the layered picture we then send as much flow as possible using
depth-first searches that start at the source and aim for the sink. The
search only steps from a chamber to a neighbour when that neighbour sits
**exactly one level deeper** and the connection between them still has
capacity. This restriction to strictly increasing levels is what keeps the
search efficient and prevents it from wandering in circles.

When a unit of flow successfully reaches the sink, the capacities along its
route are updated to reflect that this much traffic has now been committed
— the forward connections lose capacity and their reverse counterparts
gain it, so that the algorithm retains the freedom to *reroute* flow later
if a better arrangement exists. When a chamber turns out to lead nowhere
useful, the search records this by advancing that chamber's **pointer**,
so the same dead-end connection is never re-examined during the same
round. At the start of every round these pointers are reset so the fresh
layered picture can be explored from scratch.

The two stages alternate — rebuild the levels, then push all the flow that
those levels allow — until the sink can no longer be reached. The total
flow accumulated by then is the answer.

## 4. Time Complexity

Let $V$ be the number of vertices and $E$ the number of connections.

Dinic's algorithm performs a sequence of **rounds**. A crucial fact is
that each round makes the shortest source-to-sink route strictly longer
than in the previous round. Since a route can be at most $V$ connections
long, there can be at most $O(V)$ rounds. Within a single round, building
the level picture and pushing all the flow it allows together take
$O(V \cdot E)$ work. Multiplying the two factors, the general running time
of Dinic's algorithm is
$$
O(V^2 \, E).
$$

In our cave, every meaningful capacity is just $1$ and each chamber admits
only a single team, so flow moves through the network in indivisible
single units. Networks of this restricted kind allow a far stronger
guarantee: the number of rounds is only about $O(\sqrt{V})$ rather than
$O(V)$, because once the shortest route grows beyond $\sqrt{V}$ only about
$\sqrt{V}$ units of flow can remain, and each remaining round delivers at
least one of them. Each round still costs $O(E)$, so on this network the
algorithm runs in

$$
O\bigl(E \, \sqrt{V}\bigr).
$$

For an $R \times C$ grid the number of vertices and the number of
connections are both proportional to $R \cdot C$. Substituting
$V = O(RC)$ and $E = O(RC)$, the running time of the entire program is

$$
\boxed{\,O\bigl((R\,C)^{1.5}\bigr).\,}
$$

**Memory Complexity**

The graph stores two vertices per cell, so $O(RC)$ vertices in total. Each
cell contributes its single internal connection, and each grid adjacency
contributes a connection together with its reverse counterpart; since a
grid cell has at most four neighbours, the number of connections is also
proportional to $R \cdot C$. The bookkeeping arrays — one **level** value
and one **pointer** per vertex — are likewise linear in the number of
vertices. Altogether the memory used is

$$
O(R \, C).
$$

