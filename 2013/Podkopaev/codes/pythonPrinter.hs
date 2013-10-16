docFromOperation (leftOp `Sequence` rightOp) =
	(docFromOperation leftOp) <> line <> 
        docFromOperation rightOp
