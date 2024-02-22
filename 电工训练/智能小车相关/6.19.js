var initServoCfg = new Array();
                     initServoCfg[0] = 150;
                     initServoCfg[1] = 48;
                     initServoCfg[2] = 45;
                     initServoCfg[3] = 90;
                     initServoCfg[4] = 90;
                     initServoCfg[5] = 90;
                     initServoCfg[6] = 79;
                     initServoCfg[7] = 15;

bDebug = false;
simMode = 0;
var PY_VR = new Array();
PY_VR[0] = "ji ben dong zuo";
PY_VR[1] = "yao tou huang nao";
PY_VR[2] = "dian tou shi yi";
PY_VR[3] = "you hao wo zhua";
PY_VR[4] = "lan xiang fu ti";
PY_VR[5] = "ren wu zhong jie";
PY_VR[6] = "dan bu suo ding";
PY_VR[7] = "xia yi bu";
skipVRInit = true;
var baseMotionCfg = new Array();
baseMotionCfg[0] = '{"DC":true,"Dir":0,"Delay":1000,"Speak":"冲冲冲"}';
baseMotionCfg[1] = '{"DC":true,"Dir":4,"Delay":400,"Speak":"我停"}';
baseMotionCfg[2] = '{"DC":true,"Dir":1,"Delay":1000,"Speak":"退 退 退"}';
baseMotionCfg[3] = '{"DC":true,"Dir":4,"Delay":400,"Speak":"我再停"}';
baseMotionCfg[4] = '{"DC":true,"Dir":2,"Delay":500,"Speak":"左转"}';
baseMotionCfg[5] = '{"DC":true,"Dir":4,"Delay":400,"Speak":"停停停"}';
baseMotionCfg[6] = '{"DC":true,"Dir":3,"Delay":1200,"Speak":"右转"}';
baseMotionCfg[7] = '{"DC":true,"Dir":4,"Delay":1000,"Speak":"停止"}';

var nodeCameraCfg = new Array();
nodeCameraCfg[0] = '{"AC":true,"Ang":-1,"ID":8,"Delay":200,"Speak":""}';
nodeCameraCfg[1] = '{"AC":true,"Ang":15,"ID":8,"Delay":3000,"Speak":"望明月"}';
nodeCameraCfg[2] = '{"AC":true,"Ang":70,"ID":8,"Delay":3000,"Speak":"思故乡"}';
nodeCameraCfg[3] = '{"AC":true,"Ang":25,"ID":8,"Delay":3000,"Speak":"我是谁"}';
nodeCameraCfg[4] = '{"AC":true,"Ang":15,"ID":8,"Delay":3000,"Speak":"还原"}';
nodeCameraCfg[5] = '{"AC":true,"Ang":-1,"ID":8,"Delay":2000,"Speak":"我在这里啊"}';
nodeCameraCfg[6] = '{"AC":true,"Ang":-1,"ID":-1,"Delay":1000,"Speak":""}';

var shakeCameraCfg = new Array();
shakeCameraCfg[0] = '{"AC":true,"Ang":-1,"ID":7,"Delay":200,"Speak":""}';
shakeCameraCfg[1] = '{"AC":true,"Ang":120,"ID":7,"Delay":4000,"Speak":"左顾"}';
shakeCameraCfg[2] = '{"AC":true,"Ang":50,"ID":7,"Delay":4000,"Speak":"右盼"}';
shakeCameraCfg[3] = '{"AC":true,"Ang":110,"ID":7,"Delay":4000,"Speak":"东张"}';
shakeCameraCfg[4] = '{"AC":true,"Ang":60,"ID":7,"Delay":4000,"Speak":"西望"}';
shakeCameraCfg[5] = '{"AC":true,"Ang":80,"ID":7,"Delay":4000,"Speak":"还原"}';
shakeCameraCfg[6] = '{"AC":true,"Ang":-1,"ID":7,"Delay":4000,"Speak":"我在哪里呢"}';
shakeCameraCfg[7] = '{"AC":true,"Ang":-1,"ID":-1,"Delay":1000,"Speak":""}';

var claspCfg = new Array();
claspCfg[0] = '{"AC":true,"Ang":90,"ID":-2,"Delay":200,"Speak":""}';
claspCfg[1] = '{"AC":true,"Ang":100,"ID":4,"Delay":2000,"Speak":"你们好呀"}';
claspCfg[2] = '{"AC":true,"Ang":120,"ID":1,"Delay":2000,"Speak":""}';
claspCfg[3] = '{"AC":true,"Ang":140,"ID":1,"Delay":2000,"Speak":""}';
claspCfg[4] = '{"AC":true,"Ang":80,"ID":3,"Delay":2000,"Speak":"交个朋友"}';
claspCfg[5] = '{"AC":true,"Ang":145,"ID":1,"Delay":2000,"Speak":""}';
claspCfg[6] = '{"AC":true,"Ang":110,"ID":3,"Delay":2000,"Speak":""}';
claspCfg[7] = '{"AC":true,"Ang":54,"ID":3,"Delay":2000,"Speak":""}';
claspCfg[8] = '{"AC":true,"Ang":130,"ID":4,"Delay":2000,"Speak":""}';
claspCfg[9] = '{"AC":true,"Ang":140,"ID":1,"Delay":2000,"Speak":"拜拜了您嘞"}';
claspCfg[10] = '{"AC":true,"Ang":120,"ID":4,"Delay":2000,"Speak":""}';
claspCfg[11] = '{"AC":true,"Ang":120,"ID":2,"Delay":2000,"Speak":""}';
claspCfg[12] = '{"AC":true,"Ang":55,"ID":2,"Delay":2000,"Speak":""}';
claspCfg[13] = '{"AC":true,"Ang":90,"ID":-2,"Delay":200,"Speak":""}';
claspCfg[14] = '{"AC":true,"Ang":-1,"ID":-1,"Delay":1000,"Speak":""}';

var catchBallCfg = new Array();
catchBallCfg[0] = '{"AC":true,"Ang":90,"ID":-2,"Delay":200,"Speak":""}';
catchBallCfg[1] = '{"AC":true,"Ang":70,"ID":4,"Delay":2000,"Speak":"开爪子"}';
catchBallCfg[2] = '{"AC":true,"Ang":55,"ID":2,"Delay":2000,"Speak":"前伸"}';
catchBallCfg[3] = '{"AC":true,"Ang":95,"ID":1,"Delay":2000,"Speak":"低头"}';
catchBallCfg[4] = '{"AC":true,"Ang":115,"ID":4,"Delay":2000,"Speak":"抓取"}';
catchBallCfg[5] = '{"AC":true,"Ang":150,"ID":1,"Delay":2000,"Speak":"抬头"}';
catchBallCfg[6] = '{"AC":true,"Ang":80,"ID":2,"Delay":2000,"Speak":"举起"}';
catchBallCfg[7] = '{"AC":true,"Ang":10,"ID":3,"Delay":3000,"Speak":"牛不牛逼"}';
catchBallCfg[8] = '{"AC":true,"Ang":80,"ID":3,"Delay":3000,"Speak":"六不六"}';
catchBallCfg[9] = '{"AC":true,"Ang":45,"ID":3,"Delay":2000,"Speak":"水平还原"}';
catchBallCfg[10] = '{"AC":true,"Ang":55,"ID":2,"Delay":2000,"Speak":"放下"}';
catchBallCfg[11] = '{"AC":true,"Ang":95,"ID":1,"Delay":2000,"Speak":"低头"}';
catchBallCfg[12] = '{"AC":true,"Ang":45,"ID":2,"Delay":2000,"Speak":"回收"}';
catchBallCfg[13] = '{"AC":true,"Ang":85,"ID":4,"Delay":2000,"Speak":"松开"}';
catchBallCfg[14] = '{"AC":true,"Ang":90,"ID":-2,"Delay":200,"Speak":""}';
catchBallCfg[15] = '{"AC":true,"Ang":90,"ID":-2,"Delay":200,"Speak":""}';
catchBallCfg[16] = '{"AC":true,"Ang":90,"ID":-2,"Delay":200,"Speak":""}';
catchBallCfg[17] = '{"AC":true,"Ang":90,"ID":-2,"Delay":200,"Speak":""}';
catchBallCfg[18] = '{"AC":true,"Ang":90,"ID":-2,"Delay":200,"Speak":""}';
catchBallCfg[19] = '{"AC":true,"Ang":90,"ID":-2,"Delay":200,"Speak":""}';
catchBallCfg[20] = '{"AC":true,"Ang":90,"ID":-2,"Delay":2000,"Speak":""}';
catchBallCfg[21] = '{"AC":true,"Ang":90,"ID":-2,"Delay":200,"Speak":""}';
catchBallCfg[22] = '{"AC":true,"Ang":-1,"ID":-1,"Delay":1000,"Speak":""}';

var ballColor = 1;
var lock_forward = 1;
var lock_backward = 0;
var lock_turnleft = 2;
var lock_turnright = 3;
var camPos = new Array();
camPos[0] = '{"servoH":82,"servoV":21,"ballXmin":321,"ballXmax":381,"ballYmin":30,"ballYmax":90,"spdTurnFwd":40,"SPEAK":"中范围查找"}';
camPos[1] = '{"servoH":82,"servoV":38,"ballXmin":43,"ballXmax":83,"ballYmin":58,"ballYmax":98,"spdTurnFwd":20,"SPEAK":"近距离查找"}';
camPos[2] = '{"servoH":98,"servoV":53,"ballXmin":308,"ballXmax":328,"ballYmin":218,"ballYmax":238,"spdTurnFwd":1,"SPEAK":"最终定位"}';
var ballSizeMin = 113;
var ballSizeMax = 169;
var moveSpeedL = 80;
var moveSpeedR = 80;
var gearLevel_low = 1;
var lockFailBackTicks = 2000;
 
 
if($g_activeTimes<=0 || $Mod==0){
    setConfig(8,3);
    setConfig(9,65);
    setConfig(10,40);
    setConfig(11,40);
}var jsonCamPos = 0;

//变量初始化，避免误操作
$DC = false;    //车辆控制命令无效
$AC = false;    //舵机控制命令无效
$OutMod = 0;    //不改变下次工作模式

var jsonWC = JSON.parse($WC);
var jsonRC = JSON.parse($RC);
switch(ballColor){
    case 1:	jsonRC = jsonRC.R;jsonWC=jsonWC.R;break;
    case 2:	jsonRC = jsonRC.G;jsonWC=jsonWC.G;break;
    default:
    case 3:	jsonRC = jsonRC.B;jsonWC=jsonWC.B;break;
}

var nowSize = getConfig(28)*(jsonRC.w*640/getConfig(29)+jsonRC.h*480/getConfig(30))/2;
var nowPosX = getConfig(28)*jsonWC.x*640/getConfig(29);
var nowPosY = getConfig(28)*jsonWC.y*480/getConfig(30);

showWaitMsg("BallInfo:size="+nowSize+",x="+nowPosX+",y="+nowPosY);

if($g_activeTimes<=0 || $Mod==0){ //第一次运行
    setValueBase(1,-1);				//初始化8个舵机位置记忆
    setValueBase(2,-1);
    setValueBase(3,-1);
    setValueBase(4,-1);
    setValueBase(5,-1);
    setValueBase(6,-1);
    setValueBase(7,-1);
    setValueBase(8,-1);
    setValueBase(9,0);
    setValueBase(29,0);				//初始化步骤设置为0
    setValueBase(20,-1);			//上次小球信息设置为无效
    setValueBase(21,-1);
    setValueBase(22,-1);
    setValueBase(23,0);				//小球锁定配置编号设置为0
    setValueBase(24,0);				//运动命令清除
    setValueBase(26,4);				//小车状态初始为停止，仅在自动捡球模式有效
    setValueBase(27,0);				//连续找到球次数清零
    jsonCamPos = JSON.parse(camPos[0]);//解析锁球配置0
    setValueBase(30,getConfig(25));	//设置串口数据序号初值记录
    setTimeBase(0);
    if( bDebug ){
        ttsSpeak("调试模式，直接抓球");
        if(ballColor<1 || ballColor>3 ){
            initServos(10,3);
            setTimeBase(10);
        }
        else
            initServos(80,3);
        setValueBase(31,5);			//演示步骤--机械臂动作2
    }
    else{
        if( !skipVRInit ){
            if( simMode ){
                ttsSpeak("语音模式");
                initServos(99,3);
            }
            else{
                ttsSpeak("语音模式，请等候7秒钟");
                initServos(98,3);
            }
            vrConfig("{r0}");//发送配置命令使得小车退出内置流程
        }
        else{
            delayMs(1000);
            ttsSpeak("跳过配置,语音模块准备完成");
            initServos(1,3);
            setTimeBase(9);
        }
    }
}
else{
    jsonCamPos = JSON.parse(camPos[-difference(23,0)]);//解析锁球配置
    switch($Mod){
        case 70:
        case 71:
        case 72:
        case 73:
        case 74:
        case 75:
        case 76:
        case 77:
            if(elapsed(0)>100){
                $ID = $Mod-69;
                $AC = (-difference($ID,0))==initServoCfg[$Mod-70];//此几个模式只能复位，不能设置为与初始值不一样的值
                $Ang = -difference($ID,0);// initServoCfg[$Mod-70];
                $OutMod = $Mod+1;
                setTimeBase(0);
                if($AC)
                    showMsg("舵机#"+$ID+"设置为"+$Ang);
            }
            break;
        case 78://左轮速度复位
            if(elapsed(0)>100){
                $DC = true;
                $Dir = 199;
                $OutMod = 79;
                setTimeBase(0);
            }
            break;
        case 79://右轮速度复位
            if(elapsed(0)>100){
                $DC = true;
                $Dir = 299;
                $OutMod = -difference(25,0);
                setTimeBase(0);
            }
            break;

        case 98:if( elapsed(0)>7000 )	$OutMod = 99;//等待退出内置流程的语音播报完成
                else showWaitProgress(elapsed(0)/100,70);
                break;
        case 99:Init();//初始化语音模块
            break;
        case 1:	VoiceControl();	//用语音控制演示步骤
            break;
        case 2:
            setValueBase(31,1);	//演示步骤--基本动作
            $OutMod = 10;
            break;
        case 3:
            setValueBase(31,2);//演示步骤--云台动作1
            $OutMod = 10;
            break;
        case 4:
            setValueBase(31,3);//演示步骤--云台动作2
            $OutMod = 10;
            break;
        case 5:
            setValueBase(31,4);//演示步骤--机械臂动作1
            $OutMod = 10;
            break;
        case 6:
            setValueBase(31,5);//演示步骤--机械臂动作2
            $OutMod = 10;
            break;
        case 7:
            $OutMod = 10;
            break;
        case 8:
            $OutMod = 10;
            break;
        case 9:
            $OutMod = 10;
            break;
        default:DemoShow();		//执行演示
            break;
    }
    if( $OutMod != $Mod && $OutMod!=0 ){
        showMsg("工作模式变化,"+$Mod+"==>"+$OutMod+",演示步骤="+(-difference(31,0)));
    }
}

function initServos(modOut,mask)
{
    //mask==1	复位机械臂舵机
    //mask==2	复位云台舵机
    //mask==3	复位所有舵机
    showMsg("复位所有舵机，复位后的模式为"+modOut+",mask="+mask+",mask%2="+(mask%2));
    for(var i=0;i<8;i++){
        if( ((i<4)&&((mask%2)>0)) || ((i>5)&&(mask>1)) || (((i==4)||(i==5))&&(mask==3)) ){
            showMsg("复位"+(i+1)+"号舵机");
            setValueBase(i+1,initServoCfg[i]);
        }
    }
    setValueBase(25,modOut);
    $OutMod = 70;
}

function GetSerialData()
{
    var sDataIndex = getConfig(25);		//获取当前串口数据序号，即系统已收到的串口数据数。
    var sData = -1;						//无数据时返回-1
    if(-difference(30,0)!=sDataIndex){	//通过判断25号配置数据是否变化来判断串口数据是否有更新
        sData = getConfig(26);
        setValueBase(30,sDataIndex);	//更新当前串口数据序号至内置变量#30
    }
    return sData;
}

function VoiceControl()
{
    //获取串口输入，执行响应
    var sData = GetSerialData();
    //识别成功时，串口返回值为指定返回值（若未指定则返回序号）和128相或的结果
    switch(sData){
        case 128:	//动一动
            $OutMod = 2;		//演示从Mode 2开始
            break;
        case 129:	//摇摇头
            $OutMod = 3;		//演示从Mode 2开始
            break;
        case 130:	//点点头
            $OutMod = 4;		//演示从Mode 2开始
            break;
        case 131:	//握个爪
            $OutMod = 5;		//演示从Mode 2开始
            break;
        case 132:	//抓个球
            if( (ballColor<1) || (ballColor>3) )
                $OutMod = 6;		//演示从Mode 2开始，不找球直接抓
            else{
                setValueBase(31,5);//演示步骤--机械臂动作2
                setValueBase(20,-1);			//上次小球信息设置为无效
                setValueBase(21,-1);
                setValueBase(22,-1);
                setValueBase(23,0);				//小球锁定配置编号设置为0
                setValueBase(24,0);				//运动命令清除
                setValueBase(26,4);				//小车状态初始为停止，仅在自动捡球模式有效
                setValueBase(27,0);				//连续找到球次数清零
                $OutMod = 80;		//演示从Mode 80开始，自动找球然后抓球
            }
            break;
        case 133:
        case 134:
        case 135:
            ttsSpeak("命令仅在找球时有效");
            break;
        default:
            if( elapsed(9)>30000 ){
                switch(((Math.random()*Math.random()*1000)^0)%5){
                case 0:ttsSpeak("我在等候指示");break;
                case 1:ttsSpeak("主人请吩咐");break;
                case 2:ttsSpeak("不要让我傻等");break;
                case 3:ttsSpeak("请说话");break;
                default:ttsSpeak("有事启奏，无事退朝");break;
                }
                setTimeBase(9);
            }

            break;
    }
}

function DemoShow()
{
    switch(-difference(31,0)){//演示步骤
    case 1://演示小车基本运动
        BaseMotion();
        break;
    case 2://演示云台运动1
        ShakeHead();
        break;
    case 3://演示云台运动2
        NodHead();
        break;
    case 4://机械臂动作1
        GraspClaw();
        break;
    case 5://机械臂动作2
        CatchBall();
        break;
    case 6://有需要这里补充，不需要可删除
        showMsg("补充演示步骤3");
        setValueBase(31,0);//演示步骤变量复位
        setTimeBase(0);
        $OutMod = 1;	//返回语音层面
        break;
    //...
    }
}

function BaseMotion()
{
    if( $Mod<10 )	return;
    if( $Mod>baseMotionCfg.length-1+10 ){
        $OutMod = 1;//返回语音层
        return;
    }
    var cfg_str = baseMotionCfg[$Mod-10];
    var jsonCfg = JSON.parse(cfg_str);
    var jsonPreCfg = $Mod>10?JSON.parse(baseMotionCfg[$Mod-11]):"";
    var dly = $Mod>10?jsonPreCfg.Delay:600;
    if( elapsed(0)>dly && getConfig(24)==2 ){
        $DC = jsonCfg.DC;$Dir = jsonCfg.Dir;
        ttsSpeak(jsonCfg.Speak);
        setTimeBase(0);//记下当前时刻
        $OutMod = $Mod+1;
    }
    return;
}

function servoCtrl(configs)
{
    if( $Mod<10 )	return;
    if( $Mod>configs.length-1+10 ){
        $OutMod = 1;//返回语音层
        return;
    }
    var cfg_str = configs[$Mod-10];
    var jsonCfg = JSON.parse(cfg_str);
    showMsg("Mod="+$Mod+",elps(0)="+elapsed(0)+",Dleay="+jsonCfg.Delay+",vrCfg="+getConfig(24));
    showMsg("   ---ID="+jsonCfg.ID+",Ang="+jsonCfg.Ang);
    //下面的语句没有等待语音完成，因为等待语音完成会让动作不流畅。
    if( (elapsed(0)>jsonCfg.Delay) && ((getConfig(24)==2)||$Mod>10) ){
        if( jsonCfg.AC ){
            if( (jsonCfg.ID<1) || (jsonCfg.ID>8) ){
                if( jsonCfg.Ang<0 || jsonCfg.Ang>180 ){
                    //终止序列，返回语音层
                    showMsg("!!!终止控制序列,[id,ang]="+jsonCfg.ID+","+jsonCfg.Ang);
                    if( bDebug ){
                        ttsSpeak("耗时"+((elapsed(10)/1000)^0)+"秒,重新开始");
                        if( ballColor>=1 && ballColor<=3 )
                            $OutMod = 80;//1;
                        else
                            $OutMod = 10;
                        setTimeBase(10);
                        setValueBase(23,0);
                        jsonCamPos = JSON.parse(camPos[0]);
                    }
                    else{
                        if(difference(32,0)==-5){//抓球演示
                            ttsSpeak("完成抓球，耗时"+((elapsed(10)/1000)^0)+"秒");
                        }
                        else
                            ttsSpeak("完成");
                        $OutMod = 1;
                    }
                    return;
                }
                else{
                    if(jsonCfg.ID==-2){
                        //复位所有机械臂舵机
                        initServos($Mod+1,1);
                    }
                    else{
                        if(jsonCfg.ID==-3){
                            //复位所有云台舵机
                            initServos($Mod+1,2);
                        }
                        else{
                            //复位所有舵机
                            initServos($Mod+1,3);
                        }
                    }
                }
            }
            else{
                if( (jsonCfg.Ang<0) || (jsonCfg.Ang>180) ){//复位当前舵机
                    showMsg("###复位"+jsonCfg.ID+"的角度为"+(-difference($ID,0))+"=>"+initServoCfg[jsonCfg.ID-1]);
                    $AC = true;$ID = jsonCfg.ID;$Ang = initServoCfg[jsonCfg.ID-1];// (-difference(jsonCfg.ID,0));
                    setValueBase($ID,$Ang);//更新舵机位置
                }
                else{
                    showMsg("  +++设置"+jsonCfg.ID+"的目标角度为"+(-difference($ID,0))+"=>"+jsonCfg.Ang);
                    $AC = true;$ID = jsonCfg.ID;$Ang = jsonCfg.Ang;
                    setValueBase($ID,jsonCfg.Ang);//更新舵机位置
                    if($Mod+1-10<configs.length){
                        var cfg_str1 = configs[$Mod+1-10];
                        var jsonCfg1 = JSON.parse(cfg_str1);
                        updateNow("{'info':'"+jsonCfg.Ang+"==>"+jsonCfg1.Ang+"/"+jsonCfg1.Delay+"','Servo':{'Valid':false,'ID':8,'Angle':0},'DebugLed':1}");
                    }
                }
            }
        }
        if( jsonCfg.Speak!="" )
            ttsSpeak(jsonCfg.Speak);
        setTimeBase(0);//记下当前时刻
        if($OutMod==0)
            $OutMod = $Mod+1;
    }
    else{
        //每次调节一点点，使之流畅顺滑
        var dly = elapsed(0);
        if( dly<=jsonCfg.Delay ){
            if( jsonCfg.AC && (jsonCfg.ID>0) && (jsonCfg.ID<9) ){//合法调节单舵机
                var dstAng =  jsonCfg.Ang;
                if( (jsonCfg.Ang<0) || (jsonCfg.Ang>180) )//如果是舵机复位，则取初始角度为最终角度
                    dstAng = initServoCfg[jsonCfg.ID-1];
                $AC = true;$ID = jsonCfg.ID;
                $Ang = (-difference(jsonCfg.ID,0))+(dstAng-(-difference(jsonCfg.ID,0)))*dly/jsonCfg.Delay;
                showMsg("  设置"+jsonCfg.ID+"的中间角度为"+$Ang);
            }
        }
    }
}

function ShakeHead()
{
    servoCtrl(shakeCameraCfg);
}

function NodHead()
{
    servoCtrl(nodeCameraCfg);
}

function GraspClaw()
{
    servoCtrl(claspCfg);
}

function vehicleMove(mvMode)
{
    if( mvMode<0 ){//表示逆向回转
        var old = -difference(24,0);
        switch((old%10)^0){
            default:
                setValueBase(24,0);//清除运动命令
                break;
            case 2:
            case 3:
                $DC = true;
                $Dir = (-mvMode)*10+(5-((old%10)^0));
                setValueBase(24,0);//清除运动命令
                break;
        }
        return;
    }
    if( mvMode>99 )	return;
    if( -difference(26,0)==mvMode )	return;//滤除重复操作
    $DC = true;
    $Dir = mvMode;
    if( mvMode<5 )
        setValueBase(26,mvMode);
    else
        setValueBase(26,4);
    setValueBase(24,mvMode);
}


function lockBall()
{
    var sData = GetSerialData();
    switch(sData){
        case 133://任务终结
            ttsSpeak("收到，终止任务");
            $OutMod = 1;
            return;
        case 134://锁定暂停
            setValueBase(9,1+difference(9,0));
            break;
        case 135://下一步
            if( difference(9,0)!=0 && $Mod==92 ){
                $OutMod = 85;
                setTimeBase(0);
            }
            break;
    }

    //基本策略：首先在视野中找到球，使球x坐标到达位置，然后根据小球的y坐标，分两种情况：
    //1.往前边调整  2.边后退边调整

    //与是否找到球无关的状态处理
    switch($Mod){
    case 80://发送停车命令
        if( elapsed(0)>200 ){
            vehicleMove(lock_forward);
            vehicleMove(4);//强制停止
            showMsg("强制停止");
            $OutMod = 81;
            setTimeBase(0);
            if(difference(23,0)==0){
                //第一次进入定位
                setTimeBase(10);//记录演示开始时间
                ttsSpeak("开始计时");
            }
        }
        return;
    case 81://设置云台水平舵机和左轮速度
        if( elapsed(0)>200 && getConfig(24)==2 ){
            $AC = true;
            $ID = 7;
            $Ang = jsonCamPos.servoH;
            setValueBase($ID,$Ang);//更新舵机位置
            //$DC = true;
            //$Dir = 100+jsonCamPos.spdL;
            $OutMod = 89;
            setTimeBase(0);
        }
        return;
    case 86://后退重新查找状态
        if( elapsed(0)>lockFailBackTicks ){//&& getConfig(24)==2 ){
            vehicleMove(4);
            showMsg("重新开始");
            setValueBase(23,0);//重新开始锁定
            jsonCamPos = JSON.parse(camPos[-difference(23,0)]);
            $OutMod = 80;
            setTimeBase(0);
        }
        return;
    case 87:
        if( elapsed(0)<5000 ){
            $OutMod = 86;
            setTimeBase(0);
        }
        return;
    case 89://设置云台垂直舵机和右轮速度
        if( elapsed(0)>200 && getConfig(24)==2 ){
            $AC = true;
            $ID = 8;
            $Ang = jsonCamPos.servoV;
            setValueBase($ID,$Ang);//更新舵机位置
            //$DC = true;
            //$Dir = 200+jsonCamPos.spdR;
            $OutMod = 88;
            setTimeBase(0);
        }
        return;
    case 88://等待舵机动作到位，然后进入动作序列
        if( elapsed(0)>800 && getConfig(24)==2 ){
            $OutMod = 82;
            if( jsonCamPos.SPEAK.length>0 )
                ttsSpeak(jsonCamPos.SPEAK);
            //ttsSpeak("第"+(-difference(23,0)+1)+"次");
            showMsg("第"+(-difference(23,0)+1)+"次启动");
            setTimeBase(1);//记忆开始时刻
            setTimeBase(0);
        }
        return;
    case 90://复位左轮速度
        if( elapsed(0)>500 ){
            $DC = true;
            $Dir = 199;
            $OutMod = 91;
            setTimeBase(0);
        }
        return;
    case 91://复位右轮速度，结束查找流程，进入抓球
        if( elapsed(0)>500 ){
            $DC = true;
            $Dir = 299;
            $OutMod = 10;
            setTimeBase(0);
        }
        return;
    case 92://锁定暂停状态
        if( elapsed(0)>100 ){
            if( difference(9,0)==0 )
                $OutMod = 85;
            setTimeBase(0);
        }
        return;
    }


    var oldSize = -difference(22,0);
    var oldPosX = -difference(20,0);
    var oldPosY = -difference(21,0);

    var spdPwm = jsonCamPos.spdTurnFwd*(8-gearLevel_low)/100.0+gearLevel_low;	//按百分比计算的挡位，未取整
    var delayPwm = (200*(spdPwm^0)/spdPwm+0.5)^0;								//根据浮点挡位修正pwm周期时长，使挡位可以取整
    spdPwm = spdPwm^0;															//挡位取整

    //接下来的状态处理分是否找到球不同而不同
    if( nowPosX>=0 && nowPosY>=0 && nowSize>0 ){
        setValueBase(27,1-difference(27,0));//累加找到球的次数
        if( -difference(27,0)>5 )//连续找到5次以上的球才算稳定找到
            setTimeBase(1);//复位查找计时器

        //找到球的状态处理switch
        switch($Mod){
        case 82://查找阶段发现了小球，先停止小车转向，进83
            //showMsg("X:["+nowPosX+","+jsonCamPos.ballXmin+"-"+jsonCamPos.ballXmax+"],Y:["+nowPosY+","+jsonCamPos.ballYmin+"-"+jsonCamPos.ballYmax+"],sz:"+nowSize+",vrCfg:"+getConfig(24));
            if( elapsed(0)>delayPwm && getConfig(24)==2 ){
                ttsSpeak("有了");
                showMsg("有了");
                $OutMod = 83;
                setTimeBase(0);
            }
            break;
        case 83://判断球的x位置，决定左右转，到位84
            if( elapsed(0)>delayPwm && getConfig(24)==2 ){
                showWaitProgress(nowPosX,640);
                if(nowPosX<jsonCamPos.ballXmin){//太左，左转
                    vehicleMove(10+lock_turnleft);
                }
                else{
                    if(nowPosX>jsonCamPos.ballXmax){//太右，右转
                        vehicleMove(10+lock_turnright);
                    }
                    else{//OK
                        vehicleMove(4);
                        //ttsSpeak("保持距离");
                        showMsg("保持距离");
                        $OutMod = 84;
                    }
                }
                setTimeBase(0);
            }
            break;
        case 84://判断球的y位置，决定前后动，x位置不对83，双到位85
            if( elapsed(0)>delayPwm ){//&& getConfig(24)==2 ){
                showWaitProgress(nowPosY,480);
                var lstCmd = 0;
                if(nowPosY<jsonCamPos.ballYmin){//太近，后退
                    vehicleMove(10+(((jsonCamPos.ballYmin-nowPosY)/40)^0)*10+lock_backward);
                    lstCmd = 11;
                }
                else{
                    if(nowPosY>jsonCamPos.ballYmax){//太远，前进
                        //根据目标球体大小判断大致距离，然后决定前进粒度
                        var DstNowRt = ((ballSizeMin+ballSizeMax)/3)/nowSize;
                        var spdCtrl = ((((DstNowRt*spdPwm*10)^0)/10)^0)*10;
                        if( spdCtrl<10 )	spdCtrl = 10;
                        if( spdCtrl>80 )	spdCtrl = 80;
                        //这里的spdCtrl控制的是执行运动命令多久后自动执行停止命令，延时=(spdCtrl/10)*20ms
                        vehicleMove(lock_forward+spdCtrl);
                        lstCmd = 10;
                    }
                    else{//OK
                        vehicleMove(4);
                        lstCmd = 4;
                    }
                }
                if(nowPosX<jsonCamPos.ballXmin || nowPosX>jsonCamPos.ballXmax){
                    //ttsSpeak("重新搁中间");
                    showMsg("重新搁中间");
                    $OutMod = 83;
                }
                else{
                    if( lstCmd==4 ){//双到位
                        //ttsSpeak("到位");
                        vehicleMove(4);
                        showMsg("大小="+nowSize);
                        if( difference(9,0)!=0 ){
                            ttsSpeak("已暂停");
                            $OutMod = 92;
                        }
                        else
                            $OutMod = 85;
                    }
                }
                setTimeBase(0);
            }
            break;
        case 85://小球锁定状态：
                //最后一步锁定的话就判断球的大小，符合要求转抓球90，否则提示并转86(后退一段时间后重新开始)
                //非最后一步的话不用判断球大小，直接转下一步锁定(改变云台状态再次锁定小球至新位置)
            if( elapsed(0)>delayPwm ){//&& getConfig(24)==2 ){
                showMsg("锁定");
                showMsg("camPos.size="+camPos.length);
                if( (-difference(23,0))>=camPos.length-1 ){//最后一步
                    if( nowSize>=ballSizeMin && nowSize<=ballSizeMax ){
                        $OutMod = 90;
                        setValueBase(23,0);//复位锁定配置编号
                        ttsSpeak("搞定,大小为"+nowSize+",耗时"+((elapsed(10)/1000)^0)+"秒");
                        showMsg("搞定,大小为"+nowSize+",耗时"+elapsed(10)+"毫秒");
                    }
                    else{
                        //ttsSpeak("大小不符");
                        showMsg("大小不符，现大小="+nowSize);
                        showMsg("后退");
                        vehicleMove(lock_backward);//后退
                        ttsSpeak("大小不符，现大小="+nowSize);
                        $OutMod = 87;
                    }
                }
                else{//不是最后一步
                    ttsSpeak("下一步");
                    setValueBase(23,-difference(23,0)+1);//进入下一锁定
                    jsonCamPos = JSON.parse(camPos[-difference(23,0)]);
                    $OutMod = 80;
                }
                setTimeBase(0);
            }
            break;
        }
        //更新
        setValueBase(20,nowPosX);
        setValueBase(21,nowPosY);
        setValueBase(22,nowSize);
    }
    else{
        //找不到就立即清除连续找球计数器
        setValueBase(27,0);

        //没找到球的状态处理switch
        switch($Mod){
            case 80:
            case 81:
                //不要进行处理
                break;
            case 82://首先得发现指定颜色的球，找球阶段，最大延时50秒
                if(elapsed(0)>delayPwm/2 && elapsed(1)>delayPwm*100 ){
                    vehicleMove(4);
                    if(-difference(23,0)<camPos.length-1){
                        ttsSpeak("找不到?改云台配置"+(-difference(23,0)+1));
                        setValueBase(23,-difference(23,0)+1);
                        showMsg("原角度：h:"+jsonCamPos.servoH+",v:"+jsonCamPos.servoV);
                        $OutMod = 80;
                    }
                    else{
                        ttsSpeak("离开车祸现场...");
                        vehicleMove(lock_backward);
                        setValueBase(23,0);
                        $OutMod = 86;
                    }
                    jsonCamPos = JSON.parse(camPos[-difference(23,0)]);
                    showMsg(camPos[-difference(23,0)]);
                    setTimeBase(0);
                    setTimeBase(1);
                }
                else{
                    //保持一个方向优雅的转动。
                    if( elapsed(0)>delayPwm ){
                        //showMsg("seraching...");
                        vehicleMove(lock_turnleft+((spdPwm^0)*10));
                        setTimeBase(0);
                    }
                }
                break;
            default:
                //其余情况找不到球认为是偶然现象，最大延时1500ms
                if(elapsed(0)>500 && elapsed(1)>1500){//1500ms还是没球，只好重新找球
                    $OutMod = 82;
                    showMsg("球呢？球丢了");
                    setTimeBase(0);
                }
                else{
                    //什么都不做
                }
                break;
        }
    }
}

function CatchBall()
{
    if( $Mod>=80 )//先定位再抓球模式
        lockBall();
    else//直接抓球模式，假设定位完成
        servoCtrl(catchBallCfg);
}

function VR_Wait_Cfg(sCfg,next)
{
    var sData = GetSerialData();
    if( elapsed(0)>1000 || sData==198 || sData==136 ){//超时了或串口数据符合要求进入
        if( sCfg!="" )
            vrConfig(sCfg);
        setValueBase(29,next);
        setTimeBase(0);//设置计时起点
        return !(sData==198 || sData==136);//超时返回true
    }
    return false;
}

function Init(){
    //语音配置代码
    var idx100 = 0;
    var idx10 = 0;
    var idx1 = 0;
    switch(-difference(29,0)){
        case 0:	vrConfig("{c0}");
                setValueBase(29,1);
                setTimeBase(0);//设置计时起点
                break;
        case 1:	if( VR_Wait_Cfg("",2) ) setValueBase(29,100);//c0超时
                break;

        default:
                if( -difference(29,0)-2<PY_VR.length ){
                    idx100 = -difference(29,0)-2;
                    idx1 = idx100%10;
                    idx10 = (((idx100-idx1)/10)^0)%10;
                    idx100 = idx100-idx10*10-idx1;
                    if( simMode )
                        VR_Wait_Cfg("{a0"+HZ_VR[-difference(29,0)-2]+"|x"+idx100+idx10+idx1+"}",1-difference(29,0));
                    else
                        VR_Wait_Cfg("{a0"+PY_VR[-difference(29,0)-2]+"|x"+idx100+idx10+idx1+"}",1-difference(29,0));
                }
                else{
                    setTimeBase(0);
                    if( simMode )
                        VR_Wait_Cfg("{CF}",99);//更新配置,然后转99
                    else
                        VR_Wait_Cfg("{gA00}",99);//无意义操作，纯为了等待前面操作完成。然后转99
                }
                break;
        //...
        case 99:if( elapsed(0)>5000 ){
                    setValueBase(29,98);
                }
                break;
        case 98:
        case 100:if( elapsed(0)>2000 ){
                    if( -difference(29,0)!=100 )	ttsSpeak("语音模块准备完成");
                    else 							ttsSpeak("语音模块准备失败");
                    setValueBase(29,0);//恢复29号变量为0
                    setValueBase(31,0);//设置演示步骤变量初值
                    setTimeBase(9);
                    $OutMod = 1;//进入语音层
                }
                break;
    }
}
