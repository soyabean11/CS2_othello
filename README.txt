Q: Describe how and what each group member contributed for the past two weeks.

We worked on the pseudocode together on paper, and we split up the functions that we wrote. Kenny wrote the countScore, the heuristic multiplier and the functionality for choosing a move. Surya wrote the minimax function. We debugged minimax together (we had a lot of logical errors). We also added more strategies to our heuristic (eg. stable discs and frontiers.)

Q: Document the improvements that your group made to your AI to make it tournament worthy. Explain why you think your strategy(s) will work. Feel free to discuss any ideas that were tried but didn't work.

1) Stable discs - we were not able to generalise the stable discs strategy, but we improved on our heuristic to account for the fact that squares adjacent to corners are fine when the corner is filled. This was a huge over sight in the initial heuristic that just gave a negative value to such a move.

2) FRONTIERS