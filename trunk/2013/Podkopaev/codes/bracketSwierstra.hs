docFromOperation (Write exp) =
	hor1_vert ((text "write(") <> (docFromExpression exp)) 
                   (text ")")

hor1_vert :: Doc -> Doc -> Doc
hor1_vert a b = (a $$ b) <|> (element_h1 (a <> b))
