import Data.Char
import System.IO

grid_size = 20
xnum = 36
ynum = 27
theta_step = 5
tnum = 360 `div` theta_step
state_num = tnum*xnum*ynum
xy_pair = [ (iy,ix,it) | iy <- [0..(ynum-1)] , ix <- [0..(xnum-1)] , it <- [0..(tnum-1)] ]
actions = ["ccw","cw","fw"] --後ろに行く方が優先する
one_step_cost = "100"
delta_fw = 40

main = do state_trans_header
          mapM_ stateTrans xy_pair
          hPutStr stderr $ unlines $ map (\x -> stateId x ++ " 0") $ filter isFinalState xy_pair

stateTrans :: (Int,Int,Int) -> IO ()
stateTrans s = mapM_ (stateTrans' s) actions

stateTrans' s a 
 | isFinalState s = return ()
 | otherwise = putStrLn header >> stateTrans'' s a
    where header = unwords ["state",stateId s,"action",a]

isFinalState :: (Int,Int,Int) -> Bool
isFinalState (iy,ix,it) = (ix >= 11 && ix <= 15 ) && (iy <= 5)

stateTrans'' (iy,ix,it) a
 | a == "fw" = stateTransFw (iy,ix,it)
 | otherwise = putStrLn $ '\t':out
    where delta = if a == "ccw" then 1 else -1
          it' = normalizeIT (it + delta)
          s' = stateId (iy,ix,it')
          out = unwords ["state",s',"prob. 1 cost", one_step_cost]

stateTransFw :: (Int,Int,Int) -> IO ()
stateTransFw s@(iy,ix,it) = st s (1.0,(iy',ix',it))
    where theta = 2.5 + (fromIntegral (it * theta_step) ) :: Double
          theta_rad = theta * 3.141592 / 180.0
          dix = delta_fw * (cos theta_rad)
          diy = delta_fw * (sin theta_rad)
          ix' = ix + truncate (dix / grid_size)
          iy' = iy + truncate (diy / grid_size)

st :: (Int,Int,Int) -> (Double,(Int,Int,Int)) -> IO ()
st s@(iy,ix,it) (p,(iy',ix',it')) = putStrLn ('\t':out)
    where s' = if (isWall iy' ix') then stateId s else stateId (iy',ix',it')
          out = unwords ["state",s',"prob.",show p,"cost", one_step_cost]

{-- もし一つでも壁に当たったらemptyを返す --}
stCheck :: [(Double,(Int,Int,Int))] -> [(Double,(Int,Int,Int))]
stCheck es = if length cs == 0 then es else []
    where w (p,(y,x,t)) = isWall y x
          cs = filter w es

normalizeIT it = (it + tnum*100) `mod` tnum

isWall iy ix
 | ix < 2      || iy < 2           = True
 | ix >= xnum-2 || iy >= ynum-2    = True
 | ix <= 10 && iy <= 10            = True
 | ix >= 16 && ix <=28 && iy <= 10 = True
 | ix >= 7 && ix <= 19 && iy >= 16 = True
 | ix >= 25 && iy >= 16            = True
 | otherwise                       = False

state_trans_header = mapM_ putStrLn ["%%metadata%%",
    "statenum " ++ (show state_num),
    "actions " ++ unwords actions,
    "",
    "%%state transitions%%" ] 

stateId :: (Int,Int,Int) -> String
stateId (y,x,t) = show (t + x*72 + y*72*36)
