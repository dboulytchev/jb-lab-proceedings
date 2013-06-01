docFromOperation (Write exp) =
	sep[text "write(" <> (docFromExpression exp); text ")"]
