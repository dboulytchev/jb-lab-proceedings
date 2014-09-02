data Tree = T String
          | Node Tree Tree

treeToDoc :: Tree -> Doc
treeToDoc (T s)        = text s
treeToDoc (Node lt rt) = besideVariant >//< aboveVariant
  where
    p   = text "-"
    lft = treeToDoc lt
    rft = treeToDoc rt
    besideVariant = p >|< lft >|< rft
    aboveVariant  = p >-< lft >-< rft
