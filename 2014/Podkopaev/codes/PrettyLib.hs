import qualified Data.HashMap.Strict as Map
import qualified Data.List as List
import Format -- Format from Azero, Swierstra library

type Variants = Map.HashMap Frame Format
type Doc = Int -> Variants

update :: Format -> Variants -> Variants
update fmt = Map.insertWith min (fmtToFrame fmt) fmt 

checkUpdate :: Int -> Format -> Variants -> Variants
checkUpdate n f = if isSuitable n f then update f else id

text :: String -> Doc
text s n = checkUpdate n (s2fmt s) Map.empty 

indent :: Int -> Doc -> Doc
indent i d n = Map.fromList vs where
 vars = Map.elems $ Map.filter (isSuitable (n-i)) (d n)
 vs   = map ((\f -> (fmtToFrame f, f)) . indentFmt i) vars

choice, beside, above :: Doc -> Doc -> Doc
choice a b n = Map.foldl' (flip update) (b n) (a n)
beside a b   = cross besideFmt a b
above  a b   = cross aboveFmt  a b
((>//<), (>|<), (>-<)) = (choice, beside, above)

cross :: (Format -> Format -> Format) -> Doc -> Doc -> Doc
cross f a b n = Map.foldl' bFold Map.empty (a n) where
 bFold m fa = Map.foldl' (flip $ checkUpdate n . f fa) m bv
 bv         = b n

pretty :: Int -> Doc -> String
pretty n d = case Map.elems (d n) of
              [] -> error "No layout"
              xs -> (\x -> txtstr x 0 "") $ List.minimum xs
