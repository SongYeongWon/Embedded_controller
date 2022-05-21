//Window_curtain

IRQ_EXTI(){
if(is_daytime() || is_nighttime()){
	DAY_check =1 ;
}else
	DAY_check =0;
}
}

receivedCMD()

//Timer loop
if(DAY_check){
	NOW_BRIGHT=GPIOread(lightsenseor)
	if(NOW_BRIGHT	 != DAY_BRIGHT){
		DAY_cnt++;
		if(DAY_cnt>thres_second){
			usart_write(Curtain_up = now_bright){
				DAY_BIRGHT=NOW_BRIGHT
				DAY_cnt=0
				DAY_check=0
		}
		}
	}else 
	Day_cnt=0;
	Day_check=0;
}



//while loop
if(!stepperDone){
	if(CUR_UP && !CUR_OPENED) //curtain closed status
		stepperDone = stepperRun(Up) //stepperDone =1
	else if(CUR_UP && CUR_OPENED) //curtain opened status
		sttperDone = stepperRun(down) //stepperDone=1
	end
	stepperDone=1
	end	
}
//sysTick priority low 



//Door Light - INSIDE
//PIR digitalin
//EXTI

IRQ_EXTI(){
	if(is_motion){
		P_in =1;
		sendCMD(P_IN=1) // 'PI1*'
	}
}


receivedCMD()
update P_in

//timer interrupt loop

if(P_IN)
	if(P_IN_cnt<T_wait)
		L_IN=1
	P_IN_cnt++
	else
		L_IN=0;
	P_IN=0;
	P_IN_cnt=0;
	end
end
	

//DOOR LIGHT
	IRQ_IC()
		dist=getDistance()
	cnpPO++
	
	if(dist_min<dist<dist_max) P_OUT_chk ++
		if(cntPO>20)
			if(P_OUT_chk> P_OUT_thresh) P_OUT =1;
		else 													P_OUT =0;
		end
		sendCMD(P_OUT)
		P_OUT_chk=0
		cntPO=0
		end
	end
		

receivedCMD()
update p_OUT
		
		if(P_OUT) L_OUT =1
			else 		L_OUT =0
	end
			

//[SECUR MODE]
%WINDOW-BREAK
			
			//ADC TRG 0.1 sec
			IRQ_ADC_TRG()
				sound=readSound()
			if(is_winBreak(sound))
					sendCMD(SRG_TRG=1) // 'ST1'
			end
		end

%WINDOW-OPENED
				IRQ_ADC_TRG()
					winOPEN=readWinopend()
			
			if(is_winopen(winOpen)) WIN_OPEN_chk++
				if(cntWO>20)
					if(win_open_chk> win_open_thresh)
						sendCMD(SRG_TRG=1) 
					WIN_OPEN_chk=0
					cntWO=0
					end
				end
			end

					
					
receiveCMD()
		update SRG_TRG
					
		if(SRG_TRG)
			SIREN_CNT_ST=1
		end
		
		//SR_cnt=5
		if(SIREN_CNT_ST)
			if(SR_cnt ==0)
				sevensegement(SR_cnt, B=0)
			SR_cnt--
			else //SR_cnt==0
				SIREN_alarm()
			end
		end
			
		//STOP
		//EXTI
		IRQ_EXTI()
			if(STOP)
				SIREN_pause()
			end
		end
			
			
	//receievedCMD()
			
			if(MODE==0)
				
				
		
		
	
	