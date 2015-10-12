parser :: String -> AST
printer :: AST -> String
parser . printer = id
printer . parser /= id