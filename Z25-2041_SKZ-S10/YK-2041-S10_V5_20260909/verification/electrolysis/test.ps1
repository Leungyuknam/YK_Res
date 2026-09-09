$ErrorActionPreference='Stop'
$p=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$enc=[Text.Encoding]::GetEncoding(936)
function Extract-Function($rel,$name){
    $s=[IO.File]::ReadAllText((Join-Path $p $rel),$enc)
    $start=$s.IndexOf('void '+$name+'(')
    $open=$s.IndexOf('{',$start);$depth=1;$end=$open+1
    while($depth -gt 0){if($s[$end] -eq '{'){$depth++};if($s[$end] -eq '}'){$depth--};$end++}
    return $s.Substring($start,$end-$start)
}
$fields='';$methods=''
foreach($entry in @(@('c\Function\Function.c','F_mElectrolysisControl'),@('c\SenseDeal\SenseDeal.c','F_CheckWaterLevel'))){
    $s=Extract-Function $entry[0] $entry[1]
    $s=$s -creplace '\b(U8|U16)\b','int' -replace '\bxdata\s+','' -replace '\(void\)','()'
    foreach($m in [regex]::Matches($s,'static int\s+\w+\s*=\s*[^;]+;')){$fields+=$m.Value+"`n"}
    $s=[regex]::Replace($s,'static int\s+\w+\s*=\s*[^;]+;','')
    $s=$s.Replace('void '+$entry[1],'static void '+$entry[1])
    $s=$s.Replace('if(s_u8Minutes & 1)','if((s_u8Minutes & 1) != 0)')
    $s=[regex]::Replace($s,'(g_stSys\.\w+|g_bElectrolysisWaterReady) = ([01]);',{param($m) $m.Groups[1].Value+' = '+@('false','true')[[int]$m.Groups[2].Value]+';'})
    $methods+=$s+"`n"
}
$fields=$fields.Replace('static int s_u8PowerBak = 0;','static bool s_u8PowerBak = false;')
$prefix=@"
using System;
public class ElectrolysisRegression {
class Sys { public bool bTurnOn,bWaterLack,bTankRemoved,bIsAirDrying; }
static Sys g_stSys=new Sys();
static bool g_u8TestState,g_bElectrolysisWaterReady;
static bool P_HALL1,P_HALL2,P_HALL3,P_HALL4;
static int P_DJ1,P_DJ2;
const int C_DJ_IDLE=0,C_DJ_WAIT=1,C_DJ_RUN=2,C_DJ_MINUTE_TICKS=6000,C_DJ_TOTAL_MINUTES=30,C_TIME_10ms=2;
const int C_TANK_WATER_CHECK_IDLE=0,C_TANK_WATER_CHECK_WAIT=1,C_TANK_WATER_CHECK_READY=2;
static int s_u8TankWaterCheckState;
static bool tick=true;
static bool F_GetTimeTick(int flag) {return tick;}
$fields
$methods
"@
$tests=@'
static void Check(bool ok,string name) {if(!ok) throw new Exception(name);}
static void Reset() {
g_stSys=new Sys();g_u8TestState=false;g_bElectrolysisWaterReady=false;tick=true;
F_mElectrolysisControl();
s_u8TankRemoveDebounce=s_u8TankPlaceDebounce=s_u8WaterLackCnt=s_u8WaterFullCnt=s_u8ElectrolysisWaterCnt=0;
s_u8TankWaterCheckState=0;
}
static void Step(int n) {for(int i=0;i<n;i++) F_mElectrolysisControl();}
static void Start() {Reset();g_stSys.bTurnOn=true;g_bElectrolysisWaterReady=true;F_mElectrolysisControl();Check(P_DJ1==1&&P_DJ2==0,"start");}
public static string Run() {
Reset();g_stSys.bTurnOn=true;Step(1000);Check(P_DJ1==0&&P_DJ2==0,"unconfirmed startup stays off");
P_HALL1=false;P_HALL2=P_HALL3=P_HALL4=true;
for(int i=0;i<199;i++)F_CheckWaterLevel();Check(!g_bElectrolysisWaterReady,"water confirmation before 2s");
F_CheckWaterLevel();F_mElectrolysisControl();Check(P_DJ1==1,"water confirmation starts waiting task");
Start();int gaps=0;
for(int t=1;t<=180000;t++) {
F_mElectrolysisControl();Check(!(P_DJ1==1&&P_DJ2==1),"never 11");
if(t==180000)Check(P_DJ1==0&&P_DJ2==0&&s_u8State==C_DJ_IDLE,"30 minute stop");
else if(t%6000==0){gaps++;Check(P_DJ1==0&&P_DJ2==0,"minute change gap");}
else {int dir=(t/6000)%2;Check(P_DJ1==1-dir&&P_DJ2==dir,"direction at tick "+t);}
}
Check(gaps==29,"29 reversals");Step(180000);Check(P_DJ1==0&&P_DJ2==0,"completed cycle does not restart");
for(int cause=0;cause<4;cause++) {
Start();Step(1234);tick=false;
if(cause==0)g_stSys.bWaterLack=true;
if(cause==1)g_stSys.bTankRemoved=true;
if(cause==2)g_stSys.bIsAirDrying=true;
if(cause==3)g_u8TestState=true;
F_mElectrolysisControl();Check(P_DJ1==0&&P_DJ2==0,"immediate stop cause "+cause);
g_stSys.bWaterLack=g_stSys.bTankRemoved=g_stSys.bIsAirDrying=g_u8TestState=false;tick=true;Step(7000);
Check(P_DJ1==0&&P_DJ2==0,"no automatic resume cause "+cause);
g_stSys.bTurnOn=false;F_mElectrolysisControl();g_stSys.bTurnOn=true;F_mElectrolysisControl();
Check(P_DJ1==1&&s_u8Minutes==0&&s_u16MinuteTicks==0,"fresh restart cause "+cause);
}
Start();Step(6000);g_stSys.bTurnOn=false;tick=false;F_mElectrolysisControl();Check(P_DJ1==0&&P_DJ2==0,"off during gap");
Reset();g_stSys.bTurnOn=true;g_u8TestState=true;g_bElectrolysisWaterReady=true;Step(100);Check(P_DJ1==0&&P_DJ2==0,"selftest startup");
Reset();g_stSys.bTurnOn=true;P_HALL1=P_HALL2=P_HALL3=P_HALL4=true;
for(int i=0;i<300;i++){F_CheckWaterLevel();F_mElectrolysisControl();}Check(g_stSys.bTankRemoved&&!g_bElectrolysisWaterReady&&P_DJ1==0,"no tank startup");
P_HALL1=false;for(int i=0;i<199;i++){F_CheckWaterLevel();F_mElectrolysisControl();}Check(P_DJ1==0,"tank restored wait");
for(int i=0;i<60;i++){F_CheckWaterLevel();F_mElectrolysisControl();}Check(P_DJ1==1,"tank restored starts pending cycle");
P_HALL2=false;for(int i=0;i<201;i++){F_CheckWaterLevel();F_mElectrolysisControl();}Check(g_stSys.bWaterLack&&!g_bElectrolysisWaterReady&&P_DJ1==0&&P_DJ2==0,"confirmed water loss stops");
P_HALL2=true;for(int i=0;i<250;i++){F_CheckWaterLevel();F_mElectrolysisControl();}Check(!g_stSys.bWaterLack&&g_bElectrolysisWaterReady&&P_DJ1==0&&P_DJ2==0,"refill does not resume aborted cycle");
return "PASS: extracted control and water detection; 180000 timing ticks, 29 reversals, completion latch, startup debounce, tank restoration, water loss, air drying, selftest, power restart.";
}
}
'@
Add-Type -TypeDefinition ($prefix+$tests) -WarningAction SilentlyContinue
[ElectrolysisRegression]::Run()
