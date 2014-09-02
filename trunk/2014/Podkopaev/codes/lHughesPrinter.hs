docFromOperation :: Operation -> Doc

docFromOperation (varName `Assignment` exp) =
  sep[(text varName) <+> (text "="), docFromExpression exp]

docFromOperation (leftOp `Sequence` rightOp) =
  sep[(docFromOperation leftOp) <> (text ";"),
      docFromOperation rightOp]

docFromOperation (IfThenElse exp trueOp falseOp) =
  (text "if") <+> sep[(docFromExpression exp),
    (text "then") <+> (docFromOperation trueOp),
    (text "else") <+> (docFromOperation falseOp)]

docFromOperation (exp `WhileDo` loopOp) =
  (text "while") <+> sep [(docFromExpression exp),
    (text "do") <+> (docFromOperation loopOp)]

docFromOperation (Read varName) =
  (text "read(") <> (text varName) <> text(")")

docFromOperation (Write exp) =
  (text "write(") <> (docFromExpression exp) <> text(")")
