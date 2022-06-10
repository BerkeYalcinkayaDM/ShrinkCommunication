#include <xc.h>
#include "main.h"

//------------------------------------------------------------------------------------------------------------------------------------
void GetMenuList(void)
{
    unsigned char i;

    for (i=0;i<MenuItemCounts[Menu.ActiveMenu];i++) 
    {
        // Main Menu
        if (Menu.ActiveMenu==0) 
        {
            Menu.ActiveMenuList [i]= (char*)MainMenuList [Parameters.LanguageSel][i];
            Menu.ActiveMenuCodes [i]= MainMenuCodes [i];
        }
        //Dil
        else if (Menu.ActiveMenu==mnLanguage) 
        {
            Menu.ActiveMenuList [i]= (char*)LangMenuList[Parameters.LanguageSel][i];
        }
        // Timings
        else if (Menu.ActiveMenu==mnTimings) 
        {
            Menu.ActiveMenuList [i]= (char*)TimingMenuList [Parameters.LanguageSel][i];
        }
        // Tarih ve saat
        else if (Menu.ActiveMenu==mnDateTime) 
        {
            Menu.ActiveMenuList [i]= (char*)TimeMenuList [Parameters.LanguageSel][i];  // Tarih ve saat menüsü
            Menu.ActiveMenuCodes [i]= TimeMenuCodes [i];
        }
        // Bilgisayar
        else if (Menu.ActiveMenu==mnComputer) 
        {
            Menu.ActiveMenuList [i]= (char*)MenuYesNoList[Parameters.LanguageSel][i];
        }
        // mnCylOutWait
        else if (Menu.ActiveMenu==mnCylOutWait) 
        {
            Menu.ActiveMenuList [i]= (char*)MenuYesNoList[Parameters.LanguageSel][i];
        }
        // TubeTypeSource menu
        else if (Menu.ActiveMenu==mnCylTypeSource) 
        {
            if (i<=2) 
            {
                Menu.ActiveMenuList [i]= (char*)CylTypeSourceMenuList[Parameters.LanguageSel][i];
            }
            else
            {
                Menu.FMenuList[i-3][0]='F';
                sprintf(str1,"%02d",i-3);           
                Menu.FMenuList[i-3][1]=str1[0];
                Menu.FMenuList[i-3][2]=str1[1];
                Menu.FMenuList[i-3][3]=' ';
                Menu.FMenuList[i-3][4]='S';
                Menu.FMenuList[i-3][5]='H';
                Menu.FMenuList[i-3][6]='R';
                Menu.FMenuList[i-3][7]='I';
                Menu.FMenuList[i-3][8]='N';
                Menu.FMenuList[i-3][9]='G';
                Menu.FMenuList[i-3][10]=':';
                Menu.FMenuList[i-3][11]=' ';
                Menu.FMenuList[i-3][12]=0;
                
                strcat (Menu.FMenuList[i-3],MenuYesNoList[Parameters.LanguageSel][Parameters.TubeShring[i-3]]);

                Menu.ActiveMenuList [i]=  (char*)Menu.FMenuList[i-3];
            }
        }

    }

}


//------------------------------------------------------------------------------------------------------------------------------------
void PrintMenu(void)
{
    unsigned char i,j,bas,bitis;


    GLCD.Invert= 1;

    // Menu numaras?
    sprintf (str1,"%2d",Menu.MenuSel+1);
    GotoXYGLCD (110,0);
    putcGLCD (str1[0]);
    putcGLCD (str1[1]);


    if (MenuItemCounts[Menu.ActiveMenu]>5) 
    {
        // Menüyü listesini ekrana yaz
        if (Menu.MenuSel<=2) 
        {
            bas= 0;
            bitis= bas+ 4;
        }
        else if (Menu.MenuSel<=(MenuItemCounts[Menu.ActiveMenu]-3)) 
        {
            bas= Menu.MenuSel-2;
            bitis= Menu.MenuSel+ 2;
        }
        else 
        {
            bas= MenuItemCounts[Menu.ActiveMenu]-5;
            bitis= MenuItemCounts[Menu.ActiveMenu]-1;
        }
    }
    else 
    {
        bas= 0;
        bitis= MenuItemCounts[Menu.ActiveMenu]-1;
    }

    GLCD.Invert= 0;
    GLCD.Font= GLCD_FONT5x7;
    i= bas;
    j= 2;
    for (i=bas;i<=bitis;i++)
    {
        if (i==Menu.MenuSel) GLCD.Invert= 1;
            else GLCD.Invert= 0;
        putrsXYGLCD (3,j,"                    ");
        if (Menu.ActiveMenu==mnCylTypeSource) putsXYGLCD (9,j++,Menu.ActiveMenuList[i]);
        else putrsXYGLCD (9,j++,Menu.ActiveMenuList[i]);
    } // for (i=1;i<8;i++){




}



//------------------------------------------------------------------------------------------------------------------------------------
void ParamExceedMessage (void)
{
    GLCD.Font= GLCD_FONT5x7;
    GLCD.Invert= 0;
    //GotoXYGLCD (2,6);
    putrsXYGLCD (2,6,Texts[Parameters.LanguageSel].PrmLimit);
    __delay_ms(1000);
}

//------------------------------------------------------------------------------------------------------------------------------------
void ParamExceedMessage2 (void)
{
    GLCD.Font= GLCD_FONT5x7;
    GLCD.Invert= 0;
    //GotoXYGLCD (2,6);
    putrsXYGLCD (2,6,Texts[Parameters.LanguageSel].PrmLowLimit);
    __delay_ms(1000);
}

//------------------------------------------------------------------------------------------------------------------------------------
void ParamRecectedMessage (void)
{
    GLCD.Font= GLCD_FONT5x7;
    GLCD.Invert= 0;
    GotoXYGLCD (2,6);
    putrsGLCD (Texts[Parameters.LanguageSel].ParameterRejected);
    __delay_ms(1000);
}


//------------------------------------------------------------------------------------------------------------------------------------
void ParamSavedMessage (void)
{
    GLCD.Font= GLCD_FONT5x7;
    GLCD.Invert= 0;
    //GotoXYGLCD (2,6);
    putrsXYGLCD (2,6,Texts[Parameters.LanguageSel].ParameterSaved);
    __delay_ms(1000);
}


//------------------------------------------------------------------------------------------------------------------------------------
void MenuProccess (void)
{
    unsigned char x;
    unsigned int i, adr, a, b;
    
    
    
    
    ////////////////////////////////////////////////////////////////////////////////////7
    //  MENU
    //
    if (Parameters.MachineState== st_MENU) 
    {
        //----------------------------------------------------------------------
        // menü listesi (0-19 menü listesidir.)
        if (Menu.ActiveMenu<20)
        {
            if (Keys.Key2==Key_UP)
            {
                if (!Menu.MenuSel) Menu.MenuSel= MenuItemCounts[Menu.ActiveMenu]-1;
                else Menu.MenuSel--;
                PrintMenu();
            } // if (Key2==Key_Down)

            else if (Keys.Key2==Key_DOWN)
            {
                if (++Menu.MenuSel==MenuItemCounts[Menu.ActiveMenu]) Menu.MenuSel=0;
                PrintMenu();
            } // if (Key2==Key_Down
        }
        
        //----------------------------------------------------------------------
        // MAIN MENU
        if (Menu.ActiveMenu==mnMain)
        {
            if (Keys.Key2==Key_PWR)
            {
                Parameters.HeaterState= hs_HEATER_OFF;
                Parameters.MachineState= st_IDLE;
                HEATER= 0;
                PrintScreen();             
                System.CylSensorState= 0;
            }
            else if (Keys.Key2==Key_SET)
            {
                Menu.ActiveMenuBck= Menu.ActiveMenu;
                Menu.MenuSelBck= Menu.MenuSel;
                Menu.ActiveMenu= Menu.ActiveMenuCodes[Menu.MenuSel];

                // Heat Set
                if (Menu.ActiveMenu==mnHeatSet) 
                {
                   Edit.EditPassword= 0;
                   Edit.EditValue= Parameters.TemperatureSet;
                   Edit.EditX= 7;
                   Edit.EditY= 3;
                   if (Menu.Admin==1) Edit.Edit= 1;
                   Edit.EditDot= 0;
                   Edit.ActDigit= 1;
                   Edit.MaxDigit= 3;
                }
                // Pressure Set
                else if (Menu.ActiveMenu==mnPressureSet) 
                {
                   Edit.EditPassword= 0;
                   Edit.EditValue= Parameters.PressureSet;
                   Edit.EditX= 7;
                   Edit.EditY= 3;
                   if (Menu.Admin==1) Edit.Edit= 1;
                   Edit.EditDot= 3;
                   Edit.ActDigit= 2;
                   Edit.MaxDigit= 3;
                }
                // Heat Sensor
                else if (Menu.ActiveMenu==mnHeatSensor) 
                {
                   Edit.EditPassword= 0;
                   Edit.EditValue= Temperature.SensorH;
                   Edit.EditX= 7;
                   Edit.EditY= 3;
                   if (Menu.Admin==1) Edit.Edit= 1;
                   Edit.EditDot= 0;
                   Edit.ActDigit= 0;
                   Edit.MaxDigit= 3;
                }
                // Pressure Sensor
                else if (Menu.ActiveMenu==mnPresSensor) 
                {
                   Edit.EditPassword= 0;
                   Edit.EditValue= Pressure.SensorH/10;
                   Edit.EditX= 7;
                   Edit.EditY= 3;
                   if (Menu.Admin==1) Edit.Edit= 1;
                   Edit.EditDot= 0;
                   Edit.ActDigit= 2;
                   Edit.MaxDigit= 3;
                }
                // Language
                else if (Menu.ActiveMenu==mnLanguage) 
                {
                    Menu.MenuSel= Parameters.LanguageSel;
                    GetMenuList();
                    Menu.MenuSel= Parameters.LanguageSel;
                }

                 // Timings
                else if (Menu.ActiveMenu==mnTimings) 
                {
                    Menu.MenuSel= 0;
                    GetMenuList();
                }
                // Sayaclar
                else if (Menu.ActiveMenu==mnCounters) 
                {

                }
                // date and time
                else if (Menu.ActiveMenu==mnDateTime) 
                {
                   Menu.MenuSel= 0;
                   GetMenuList();
                }
                // Computer
                else if (Menu.ActiveMenu==mnComputer) 
                {
                    Menu.MenuSel= Parameters.Computer;
                    GetMenuList();
                }
                // mnCylOutWait
                else if (Menu.ActiveMenu==mnCylOutWait) 
                {
                    Menu.MenuSel= Parameters.CylOutWait;
                    GetMenuList();
                }
                // mnCylTypeSource
                else if (Menu.ActiveMenu==mnCylTypeSource) 
                {
                    Menu.MenuSel= Parameters.CylinderSelect;
                    GetMenuList();
                }
                // Heat Limit
                if (Menu.ActiveMenu==mnHeatLimit) 
                {
                   Edit.EditPassword= 0;
                   Edit.EditValue= Parameters.EvacuationTemp;
                   Edit.EditX= 7;
                   Edit.EditY= 3;
                   if (Menu.Admin==1) Edit.Edit= 1;
                   Edit.EditDot= 0;
                   Edit.ActDigit= 1;
                   Edit.MaxDigit= 3;
                }
                // TempLowSet
                if (Menu.ActiveMenu==mnTempLowSet) 
                {
                   Edit.EditPassword= 0;
                   Edit.EditValue= Parameters.TemperatureLowSet;
                   Edit.EditX= 7;
                   Edit.EditY= 3;
                   if (Menu.Admin==1) Edit.Edit= 1;
                   Edit.EditDot= 0;
                   Edit.ActDigit= 1;
                   Edit.MaxDigit= 3;
                }
//                else if (Menu.ActiveMenu==mnExit)
//                {
//                    Parameters.HeaterState= hs_HEATER_OFF;
//                    Parameters.MachineState= st_IDLE;
//                    PrintScreen();
//                }                    
                else 
                {
                 //   Menu.MenuSel= 0;
                 //   GetMenuList();
                }
                PrintScreen();             
            }            
        } // MAIN MENU

        
        //----------------------------------------------------------------------
        // mnHeatSet
        else if (Menu.ActiveMenu==mnHeatSet)
        {
            if (Keys.Key2==Key_SET) 
            {
                if (Edit.ActDigit==Edit.MaxDigit)
                {
                    if (Edit.EditValue>TEMP_LIMIT2)
                    {
                        ParamExceedMessage ();
                    }
                    else
                    {
                        if (Menu.Admin==1)
                        {
                            Parameters.TemperatureSet= Edit.EditValue;

                            if(ShrinkCommunicationTransmit(sh_cmd_SET, 
                                                           sh_prm_HEAT_SET, 
                                                           Parameters.TemperatureSet) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                            } 
                            
                            Write_i_eep (TemperatureSet_adr, Parameters.TemperatureSet); // parametreyi kaydet                    
                            ParamSavedMessage();                                                
                        }
                    }
                        
                    Menu.ActiveMenu= Menu.ActiveMenuBck;
                    Menu.MenuSel= Menu.MenuSelBck;
                    Edit.Edit= 0;
                    PrintScreen ();
                }
            }              
            else if (Keys.Key2==Key_PWR) 
            {
                Menu.MenuSel= Menu.MenuSelBck;
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                Edit.Edit= 0;                
                PrintScreen();
                __delay_ms (100);
            }
        } // 
        
        //----------------------------------------------------------------------
        // mnPressureSet
        else if (Menu.ActiveMenu==mnPressureSet)
        {
            if (Keys.Key2==Key_SET) 
            {
                if (Edit.ActDigit==Edit.MaxDigit)
                {
                    if (Edit.EditValue>PRESS_LIMIT)
                    {
                        ParamExceedMessage ();
                    }
                    else if (Edit.EditValue<5)
                    {
                        ParamExceedMessage2 ();
                    }                    
                    else
                    {
                        if (Menu.Admin==1)
                        {
                            Parameters.PressureSet= Edit.EditValue;

                            if(ShrinkCommunicationTransmit(sh_cmd_SET, 
                                                           sh_prm_PRESSURE_SET, 
                                                           Parameters.PressureSet) == OK)
                            {
                                shrinkCommunicationTypeDef.txIsOk = RESET;
                                shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                                shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                                // Gönderildi mesaji
                            } 
                            
                            Write_i_eep (PressureSet_adr, Parameters.PressureSet); // parametreyi kaydet                    
                            ParamSavedMessage();                                                
                        }
                    }
                        
                    Menu.ActiveMenu= Menu.ActiveMenuBck;
                    Menu.MenuSel= Menu.MenuSelBck;
                    Edit.Edit= 0;
                    PrintScreen ();
                }
            }              
            else if (Keys.Key2==Key_PWR) 
            {
                Menu.MenuSel= Menu.MenuSelBck;
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                Edit.Edit= 0;                
                PrintScreen();
                __delay_ms (100);
            }
        } // 

        //----------------------------------------------------------------------
        // mnHeatSensor
        else if (Menu.ActiveMenu==mnHeatSensor)
        {
            if (Keys.Key2==Key_SET) 
            {
                if (Edit.ActDigit==Edit.MaxDigit)
                {
                    if (Menu.Admin==1)
                    {
                        Temperature.SensorH= Edit.EditValue;
                        
                        if(ShrinkCommunicationTransmit(sh_cmd_SET, 
                                                       sh_prm_MAX_HEAT, 
                                                       Temperature.SensorH) == OK)
                        {
                            shrinkCommunicationTypeDef.txIsOk = RESET;
                            shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                            shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                            // Gönderildi mesaji
                        }                         
                        
                        Write_i_eep (TempSensorH_adr, Edit.EditValue); // parametreyi kaydet                    
                        ParamSavedMessage();                                                
                        CalcSensorsConst();
                    }    
                    
                    Menu.ActiveMenu= Menu.ActiveMenuBck;
                    Menu.MenuSel= Menu.MenuSelBck;
                    Edit.Edit= 0;
                    PrintScreen ();
                }
            }              
            else if (Keys.Key2==Key_PWR) 
            {
                Menu.MenuSel= Menu.MenuSelBck;
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                Edit.Edit= 0;                
                PrintScreen();
                __delay_ms (100);
            }
        } // 

        //----------------------------------------------------------------------
        // mnPresSensor
        else if (Menu.ActiveMenu==mnPresSensor)
        {
            if (Keys.Key2==Key_SET) 
            {
                if (Edit.ActDigit==Edit.MaxDigit)
                {                    
                    if (Menu.Admin==1)
                    {
                        Pressure.SensorH= Edit.EditValue*10;
                        
                        if(ShrinkCommunicationTransmit(sh_cmd_SET, 
                                                       sh_prm_MAX_PRESSURE, 
                                                       Temperature.SensorH) == OK)
                        {
                            shrinkCommunicationTypeDef.txIsOk = RESET;
                            shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                            shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                            // Gönderildi mesaji
                        }                        
                        
                        Write_i_eep (PressureSensorH_adr, Edit.EditValue*10); // parametreyi kaydet                    
                        ParamSavedMessage();                                                
                        CalcSensorsConst();
                    }    
                    
                    Menu.ActiveMenu= Menu.ActiveMenuBck;
                    Menu.MenuSel= Menu.MenuSelBck;
                    Edit.Edit= 0;
                    PrintScreen ();
                }
            }              
            else if (Keys.Key2==Key_PWR) 
            {
                Menu.MenuSel= Menu.MenuSelBck;
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                Edit.Edit= 0;                
                PrintScreen();
                __delay_ms (100);
            }
        } // 

        //----------------------------------------------------------------------
        // LANGUAGE
        else if (Menu.ActiveMenu==mnLanguage)
        {
            if (Keys.Key2==Key_SET)
            {
//                if (Menu.MenuSel==0)
//                {
//                    Menu.ActiveMenu= Menu.ActiveMenuBck;
//                    Menu.MenuSel= Menu.MenuSelBck;
//                    GetMenuList();
//                    PrintScreen ();                                    
//                }
//                else
                {
                    Parameters.LanguageSel= Menu.MenuSel;
                    Write_i_eep (LanguageSel_adr,(unsigned int)Parameters.LanguageSel);
                    ParamSavedMessage();
                }
                Menu.MenuSel= Menu.MenuSelBck;
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                GetMenuList();
                PrintScreen ();
            }
            else if (Keys.Key2==Key_PWR)
            {
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                Menu.MenuSel= Menu.MenuSelBck;
                GetMenuList();
                PrintScreen ();                
            }
        } // LANGUAGE

        //----------------------------------------------------------------------
        // TIMINGS
        else if (Menu.ActiveMenu==mnTimings)
        {
            if (Keys.Key2==Key_SET)
            {
//                if (Menu.MenuSel==0)
//                {
//                    Menu.ActiveMenu= Menu.ActiveMenuBck;
//                    Menu.MenuSel= Menu.MenuSelBck;
//                    GetMenuList();
//                    PrintScreen ();                                    
//                }
//                else
                {
                    Menu.ActiveMenuBck2= Menu.ActiveMenu;
                    Menu.MenuSelBck2= Menu.MenuSel;
                    Menu.ActiveMenu= mnT1+ Menu.MenuSel;

                    if (Menu.ActiveMenu== mnT1) Edit.EditValue= Parameters.T1/2;
                    else if (Menu.ActiveMenu== mnT2) Edit.EditValue= Parameters.T2/2;
                    else if (Menu.ActiveMenu== mnT3) Edit.EditValue= Parameters.T3/2;
                    else if (Menu.ActiveMenu== mnT4) Edit.EditValue= Parameters.T4/2;
                    else if (Menu.ActiveMenu== mnT5) Edit.EditValue= Parameters.T5/2;

                    Edit.EditPassword= 0;                   
                    Edit.EditDot= 3;
                    Edit.ActDigit= 0;
                    Edit.MaxDigit= 3;
                    Edit.EditX= 7;
                    Edit.EditY= 3;
                    Edit.Edit= 1;
                    PrintScreen();
                }
            }
            else if (Keys.Key2==Key_PWR)
            {
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                Menu.MenuSel= Menu.MenuSelBck;
                GetMenuList();
                PrintScreen ();                
            }            
        } // TIMINGS
        
        //----------------------------------------------------------------------
        // T1-T5
        else if ((Menu.ActiveMenu>=mnT1)&&(Menu.ActiveMenu<=mnT6))
        {
            if (Keys.Key2==Key_SET)
            {
                if (Edit.ActDigit==Edit.MaxDigit)
                {
                    if (Menu.ActiveMenu==mnT1) 
                    {
                        Parameters.T1= Edit.EditValue*2;
                        
                        if(ShrinkCommunicationTransmit(sh_cmd_TIMINGS, 
                                                       sh_prm_T1, 
                                                       Parameters.T1) == OK)
                        {
                            shrinkCommunicationTypeDef.txIsOk = RESET;
                            shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                            shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                            // Gönderildi mesaji
                        }
                        
                        Write_i_eep(T1_adr, Parameters.T1);
                    }
                    else if (Menu.ActiveMenu==mnT2) 
                    {
                        Parameters.T2= Edit.EditValue*2;
                        
                        if(ShrinkCommunicationTransmit(sh_cmd_TIMINGS, 
                                                       sh_prm_T2, 
                                                       Parameters.T2) == OK)
                        {
                            shrinkCommunicationTypeDef.txIsOk = RESET;
                            shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                            shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                            // Gönderildi mesaji
                        }                        
                        
                        Write_i_eep(T2_adr, Parameters.T2);
                    }
                    else if (Menu.ActiveMenu==mnT3) 
                    {
                        Parameters.T3= Edit.EditValue*2;
                        
                        if(ShrinkCommunicationTransmit(sh_cmd_TIMINGS, 
                                                       sh_prm_T3, 
                                                       Parameters.T3) == OK)
                        {
                            shrinkCommunicationTypeDef.txIsOk = RESET;
                            shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                            shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                            // Gönderildi mesaji
                        }                           
                        
                        Write_i_eep(T3_adr, Parameters.T3);
                    }
                    else if (Menu.ActiveMenu==mnT4) 
                    {
                        Parameters.T4= Edit.EditValue*2;
                        
                        if(ShrinkCommunicationTransmit(sh_cmd_TIMINGS, 
                                                       sh_prm_T4, 
                                                       Parameters.T4) == OK)
                        {
                            shrinkCommunicationTypeDef.txIsOk = RESET;
                            shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                            shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                            // Gönderildi mesaji
                        }                           
                        
                        Write_i_eep(T4_adr, Parameters.T4);
                    }
                    else if (Menu.ActiveMenu==mnT5) 
                    {
                        Parameters.T5= Edit.EditValue*2;
                        
                        if(ShrinkCommunicationTransmit(sh_cmd_TIMINGS, 
                                                       sh_prm_T5, 
                                                       Parameters.T5) == OK)
                        {
                            shrinkCommunicationTypeDef.txIsOk = RESET;
                            shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                            shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                            // Gönderildi mesaji
                        }                           
                        
                        Write_i_eep(T5_adr, Parameters.T5);
                    }
                    ParamSavedMessage();
                    Edit.Edit=0;
                    Menu.ActiveMenu= Menu.ActiveMenuBck2;
                    PrintScreen ();                
                }
            }
            else if (Keys.Key2==Key_PWR)
            {
                Menu.ActiveMenu= Menu.ActiveMenuBck2;
                Edit.Edit=0;
                GetMenuList();
                PrintScreen ();                
            }            
        } // T1-T5
        
        //----------------------------------------------------------------------
        // COUNTERS
        else if (Menu.ActiveMenu==mnCounters)
        {
            
            if ((Keys.Key2==Key_UP)&&(Keys.KeyPressTime>200))
            {

                Parameters.TodayCount=0;;
                Write_i_eep (TodayCounter_adr,(unsigned int)0); 
                PrintScreen();
                while (Keys.KeyPressed);
                __delay_ms (40); // key debouncing 50ms
                return;
            } 
            else if ((Keys.Key2==Key_DOWN)&&(Keys.KeyPressTime>200))
            {

                Parameters.TotalCount=0;;
                Write_i_eep (TotalCounter_adr,(unsigned int)0); 
                PrintScreen();
                while (Keys.KeyPressed);
                __delay_ms (40); // key debouncing 50ms
                return;
            } 
            else if (Keys.Key2==Key_PWR)
            {
                Menu.MenuSel= Menu.MenuSelBck;
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                Edit.Edit= 0;
                PrintScreen();
            } 
        } // COUNTERS

        //----------------------------------------------------------------------
        // DATE AND TIME
        else if (Menu.ActiveMenu==mnDateTime)
        {
            if (Keys.Key2==Key_SET)
            {
//                if (Menu.MenuSel==0)
//                {
//                    Menu.ActiveMenu= Menu.ActiveMenuBck;
//                    Menu.MenuSel= Menu.MenuSelBck;
//                    GetMenuList();
//                    PrintScreen ();                                    
//                }
//                else
                {
                    Menu.ActiveMenuBck2= Menu.ActiveMenu;
                    if (Menu.MenuSel==0) Menu.ActiveMenu= mnDate;
                    else if (Menu.MenuSel==1) Menu.ActiveMenu= mnTime;
                    Menu.MenuSelBck2= Menu.MenuSel;
                    if (Menu.ActiveMenu== mnDate) 
                    {   // date
                        Edit.EditDateTime= 1;
                        Edit.MaxDigit= 9;

                        RTCGetDate((unsigned char *)DateTime.Day,(unsigned char *)DateTime.Month,(unsigned char *)DateTime.Year,(unsigned char *)DateTime.DOW);
                        sprintf (str1,"%02d",DateTime.Day);
                        //Str1Duzenle();
                        str2[0]= str1[0];
                        str2[1]= str1[1];
                        str2[2]='/';
                        sprintf (str1,"%02d",DateTime.Month);
                        //Str1Duzenle();
                        str2[3]= str1[0];
                        str2[4]= str1[1];
                        str2[5]='/';
                        sprintf (str1,"%02d",DateTime.Year);
                        //Str1Duzenle();
                        str2[6]= '2';
                        str2[7]= '0';
                        str2[8]= str1[0];
                        str2[9]= str1[1];
                        str2[10]=0;
                    }
                    if (Menu.ActiveMenu==mnTime) 
                    {   // time
                        Edit.EditDateTime= 1;
                        Edit.MaxDigit= 4;

                        RTCGetTime((unsigned char *)DateTime.Hour, (unsigned char *)DateTime.Minute, (unsigned char *)DateTime.Second);
                        sprintf (str1,"%02d",DateTime.Hour);
                        //Str1Duzenle();
                        str2[0]= str1[0];
                        str2[1]= str1[1];
                        str2[2]=':';
                        sprintf (str1,"%02d",DateTime.Minute);
                        //Str1Duzenle();
                        str2[3]= str1[0];
                        str2[4]= str1[1];
                        str2[5]= 0;
                    }
                    Edit.EditX= 10;
                    Edit.EditY= 4;
                    Edit.EditDot= 0;
                    Edit.EditPassword= 0;
                    Edit.ActDigit= 0;
                    PrintScreen();
                }
            }
            else if (Keys.Key2==Key_PWR)
            {
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                Menu.MenuSel= Menu.MenuSelBck;
                GetMenuList();
                PrintScreen ();                
            }
        } // DATE AND TIME
        
        
        //----------------------------------------------------------------------
        // DATE
        else if (Menu.ActiveMenu==mnDate)
        {            
            if (Keys.Key2==Key_SET)
            {
                if (Edit.ActDigit==Edit.MaxDigit)
                {
                    // gün
                    str1[0]= str2[0];
                    str1[1]= str2[1];
                    str1[2]= 0;
                    DateTime.Day= atoi (str1);
                    // ay
                    str1[0]= str2[3];
                    str1[1]= str2[4];
                    str1[2]= 0;
                    DateTime.Month= atoi (str1);
                    // y?l
                    str1[0]= str2[8];
                    str1[1]= str2[9];
                    str1[2]= 0;
                    DateTime.Year= atoi (str1);

                    // gün 31 den büyük almayacak
                    if ((DateTime.Month==1)||(DateTime.Month==3)||(DateTime.Month==5)||(DateTime.Month==7)||(DateTime.Month==8)||(DateTime.Month==10)||(DateTime.Month==12)) {
                        if (DateTime.Day>31) {
                            putrsXYGLCD (10,6,Texts[Parameters.LanguageSel].DateError);
                            __delay_ms(1000);
                            PrintScreen();
                            return;
                        }
                    }
                    // gün 30 den büyük almayacak
                    if ((DateTime.Month==4)||(DateTime.Month==6)||(DateTime.Month==9)||(DateTime.Month==11)) {
                        if (DateTime.Day>30) {
                            putrsXYGLCD (10,6,Texts[Parameters.LanguageSel].DateError);
                            __delay_ms(1000);
                            PrintScreen();
                            return;
                        }
                    }
                    // gün 29 dan büyük olmayacak
                    if (DateTime.Month==2) {
                        if (DateTime.Day>29) {
                            putrsXYGLCD (10,6,Texts[Parameters.LanguageSel].DateError);
                            __delay_ms(1000);
                            PrintScreen();
                            return;
                        }
                    }
                    if ((DateTime.Month>12)||(DateTime.Month<1)) {
                        putrsXYGLCD (10,6,Texts[Parameters.LanguageSel].DateError);
                        __delay_ms(1000);
                        PrintScreen();
                        return;
                    }
                    if (DateTime.Day<1) {
                        putrsXYGLCD (10,6,Texts[Parameters.LanguageSel].DateError);
                        __delay_ms(1000);
                        PrintScreen();
                        return;
                    }                                   
                    RTCSetDate(DateTime.Day,DateTime.Month,DateTime.Year,0);
                    ParamSavedMessage();                
                    Menu.ActiveMenu= Menu.ActiveMenuBck2;
                    Menu.MenuSel= Menu.MenuSelBck2;
                    Edit.EditDateTime=0;
                    PrintScreen();
                }
            }
            else if (Keys.Key2==Key_PWR)
            {
                Menu.ActiveMenu= Menu.ActiveMenuBck2;
                Menu.MenuSel= Menu.MenuSelBck2;
                Edit.EditDateTime=0;                
                PrintScreen ();                
            }
        } // DATE

        
        //----------------------------------------------------------------------
        // TIME
        else if (Menu.ActiveMenu==mnTime)
        {            
            if (Keys.Key2==Key_SET)
            {
                if (Edit.ActDigit==Edit.MaxDigit)
                {
                    // saat
                    str1[0]= str2[0];
                    str1[1]= str2[1];
                    str1[2]= 0;
                    DateTime.Hour= atoi (str1);
                    // dakika
                    str1[0]= str2[3];
                    str1[1]= str2[4];
                    str1[2]= 0;
                    DateTime.Minute= atoi (str1);

                    if (DateTime.Hour>23) {
                        putrsXYGLCD (10,6,Texts[Parameters.LanguageSel].TimeError);
                        __delay_ms(1000);
                        PrintScreen();
                        return;
                    }
                    if (DateTime.Minute>59) {
                        putrsXYGLCD (10,6,Texts[Parameters.LanguageSel].TimeError);
                        __delay_ms(1000);
                        PrintScreen();
                        return;
                    }
                    RTCSetDateTime(DateTime.Day,DateTime.Month,DateTime.Year,0,DateTime.Hour,DateTime.Minute);
                    ParamSavedMessage();                
                    Menu.ActiveMenu= Menu.ActiveMenuBck2;
                    Menu.MenuSel= Menu.MenuSelBck2;
                    Edit.EditDateTime=0;
                    PrintScreen();
                }
            }
            else if (Keys.Key2==Key_PWR)
            {
                Menu.ActiveMenu= Menu.ActiveMenuBck2;
                Menu.MenuSel= Menu.MenuSelBck2;
                Edit.EditDateTime=0;                
                PrintScreen ();                
            }
        } // TIME

        //----------------------------------------------------------------------
        // mnComputer
        else if (Menu.ActiveMenu==mnComputer)
        {
            if (Keys.Key2==Key_SET)
            {
//                if (Menu.MenuSel==0)
//                {
//                    Menu.ActiveMenu= Menu.ActiveMenuBck;
//                    Menu.MenuSel= Menu.MenuSelBck;
//                    GetMenuList();
//                    PrintScreen ();                                    
//                }
//                else
                {
                    Parameters.Computer= Menu.MenuSel;
                    Write_i_eep (Computer_adr,(unsigned int)Parameters.Computer);
                    ParamSavedMessage();
                    Menu.MenuSel= Menu.MenuSelBck;
                    Menu.ActiveMenu= Menu.ActiveMenuBck;
                    GetMenuList();
                    PrintScreen ();
                }
            }
            else if (Keys.Key2==Key_PWR)
            {
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                Menu.MenuSel= Menu.MenuSelBck;
                GetMenuList();
                PrintScreen ();                
            }
        } //  
        
        //----------------------------------------------------------------------
        // CylOutWait
        else if (Menu.ActiveMenu==mnCylOutWait)
        {
            if (Keys.Key2==Key_SET)
            {
//                if (Menu.MenuSel==0)
//                {
//                    Menu.ActiveMenu= Menu.ActiveMenuBck;
//                    Menu.MenuSel= Menu.MenuSelBck;
//                    GetMenuList();
//                    PrintScreen ();                                    
//                }
//                else
                {
                    Parameters.CylOutWait= Menu.MenuSel;
                    Write_i_eep (CylOutWait_adr,(unsigned int)Parameters.CylOutWait);
                    ParamSavedMessage();
                    Menu.MenuSel= Menu.MenuSelBck;
                    Menu.ActiveMenu= Menu.ActiveMenuBck;
                    GetMenuList();
                    PrintScreen ();
                }
            }
            else if (Keys.Key2==Key_PWR)
            {
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                Menu.MenuSel= Menu.MenuSelBck;
                GetMenuList();
                PrintScreen ();                
            }
        } //  

        //----------------------------------------------------------------------
        // mnCylTypeSource 
        else if (Menu.ActiveMenu==mnCylTypeSource)
        {
           if (Keys.Key2==Key_SET)
            {
//                if (Menu.MenuSel==0)
//                {
//                    Menu.ActiveMenu= Menu.ActiveMenuBck;
//                    Menu.MenuSel= Menu.MenuSelBck;
//                    GetMenuList();
//                    PrintScreen ();                                    
//                }
//                else
                {
                    if (Menu.MenuSel==0) // YOK
                    {
                        Parameters.CylinderSelect= Menu.MenuSel-1;
                        Write_i_eep (CylinderSel_adr,(unsigned int)Parameters.CylinderSelect);                   
                        MenuItemCounts[mnCylTypeSource]= 4;
                        ParamSavedMessage();
                        Menu.ActiveMenu= Menu.ActiveMenuBck;
                        Menu.MenuSel= Menu.MenuSelBck;                    
                        GetMenuList();
                        PrintScreen();
                    }
                    else if (Menu.MenuSel==1) // LEAK DETECTOR
                    {
                        Parameters.CylinderSelect= Menu.MenuSel-1;
                        Write_i_eep (CylinderSel_adr,(unsigned int)Parameters.CylinderSelect);                   
                        MenuItemCounts[mnCylTypeSource]= 14;
                        ParamSavedMessage();
                        Menu.ActiveMenu= Menu.ActiveMenuBck;
                        Menu.MenuSel= Menu.MenuSelBck;
                        GetMenuList();
                        PrintScreen();
                    }
                    else if (Menu.MenuSel==2) // CheckScale
                    {
                        Parameters.CylinderSelect= Menu.MenuSel-1;
                        Write_i_eep (CylinderSel_adr,(unsigned int)Parameters.CylinderSelect);                   
                        MenuItemCounts[mnCylTypeSource]= 14;
                        ParamSavedMessage();
                        Menu.ActiveMenu= Menu.ActiveMenuBck;
                        Menu.MenuSel= Menu.MenuSelBck;                  
                        GetMenuList();
                        PrintScreen();
                    }
                    else if ((Menu.MenuSel>=3)&&(Menu.MenuSel<=13))
                    {                  
                        if (Parameters.TubeShring[Menu.MenuSel-3]==0) Parameters.TubeShring[Menu.MenuSel-3]=1;
                            else Parameters.TubeShring[Menu.MenuSel-3]= 0;
                        Write_i_eep (F1_Shring_adr+(Menu.MenuSel-3)*2,(unsigned int)Parameters.TubeShring[Menu.MenuSel-3]);
                        GetMenuList();
                        PrintScreen();
                    }
                }            
            }
            else if (Keys.Key2==Key_PWR)
            {
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                Menu.MenuSel= Menu.MenuSelBck;                    
                GetMenuList();
                PrintScreen ();                
            }
        } // AUTO CYLINDER HEIGHT

        //----------------------------------------------------------------------
        // mnHeatLimit
        else if (Menu.ActiveMenu==mnHeatLimit)
        {
            if (Keys.Key2==Key_SET) 
            {
                if (Edit.ActDigit==Edit.MaxDigit)
                {
                    if (Edit.EditValue>TEMP_LIMIT)
                    {
                        ParamExceedMessage ();
                    }
                    else
                    {
                        if (Menu.Admin==1)
                        {
                            Parameters.EvacuationTemp= Edit.EditValue;
                            
                        if(ShrinkCommunicationTransmit(sh_cmd_SET, 
                                                       sh_prm_HEAT_LIMIT, 
                                                       Parameters.EvacuationTemp) == OK)
                        {
                            shrinkCommunicationTypeDef.txIsOk = RESET;
                            shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                            shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                            // Gönderildi mesaji
                        }                             
                            
                            Write_i_eep (EvacuationTemp_adr, Parameters.EvacuationTemp); // parametreyi kaydet                    
                            ParamSavedMessage();                                                
                        }
                    }
                        
                    Menu.ActiveMenu= Menu.ActiveMenuBck;
                    Menu.MenuSel= Menu.MenuSelBck;
                    Edit.Edit= 0;
                    PrintScreen ();
                }
            }              
            else if (Keys.Key2==Key_PWR) 
            {
                Menu.MenuSel= Menu.MenuSelBck;
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                Edit.Edit= 0;                
                PrintScreen();
                __delay_ms (100);
            }
        } // 
        
        //----------------------------------------------------------------------
        // mnTempLowSet
        else if (Menu.ActiveMenu==mnTempLowSet)
        {
            if (Keys.Key2==Key_SET) 
            {
                if (Edit.ActDigit==Edit.MaxDigit)
                {
                    if (Menu.Admin==1)
                    {
                        Parameters.TemperatureLowSet= Edit.EditValue;
                        
                        if(ShrinkCommunicationTransmit(sh_cmd_SET, 
                                                       sh_prm_HEAT_HIST, 
                                                       Parameters.TemperatureLowSet) == OK)
                        {
                            shrinkCommunicationTypeDef.txIsOk = RESET;
                            shrinkCommunicationTypeDef.shrinkCommunicationStatus = sh_st_WAIT_ACK;
                            shrinkCommunicationTypeDef.shrinkCommunicationTimeout = COMMUNICATION_TIMEOUT;
                            // Gönderildi mesaji
                        }                       
                        
                        Write_i_eep (TemperatureLowSet_adr, Parameters.TemperatureLowSet); // parametreyi kaydet                    
                        ParamSavedMessage();                                                
                    }
                    
                    Menu.ActiveMenu= Menu.ActiveMenuBck;
                    Menu.MenuSel= Menu.MenuSelBck;
                    Edit.Edit= 0;
                    PrintScreen ();
                }
            }              
            else if (Keys.Key2==Key_PWR) 
            {
                Menu.MenuSel= Menu.MenuSelBck;
                Menu.ActiveMenu= Menu.ActiveMenuBck;
                Edit.Edit= 0;                
                PrintScreen();
                __delay_ms (100);
            }
        } // 
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        







        
        
        
        
        
        
        
        
        




/*

        //----------------------------------------------------------------------
        // AUTO CYLINDER HEIGHT
        else if (Menu.ActiveMenu==mnAutoCylHeight)
        {
            if (Keys.Key2==Key_Enter)
            {
                if (Menu.MenuSel==0) // YES
                {
                    Parameters.AutoCylHeight= ynYES;
                    Write_i_eep (AutoCylHeight_adr,(unsigned int)Parameters.AutoCylHeight);                   
                    MenuItemCounts[6]= 12;
                    GetMenuList();
                    PrintScreen();
                }
                else if (Menu.MenuSel==1) // NO
                {
                    Parameters.AutoCylHeight= ynNO;
                    Write_i_eep (AutoCylHeight_adr,(unsigned int)Parameters.AutoCylHeight);                   
                    MenuItemCounts[6]= 2;
                    GetMenuList();
                    PrintScreen();
                }
                else if ((Menu.MenuSel>=2)&&(Menu.MenuSel<=11))
                {                  
                    if (Parameters.TubeHeight[Menu.MenuSel-2]==0) Parameters.TubeHeight[Menu.MenuSel-2]=1;
                        else Parameters.TubeHeight[Menu.MenuSel-2]= 0;
                                
                    Write_i_eep (F1_Height_adr+(Menu.MenuSel-2)*2,(unsigned int)Parameters.TubeHeight[Menu.MenuSel-2]);
                    
                    GetMenuList();
                    PrintScreen();
                }
            }
            else if (Keys.Key2==Key_Clr)
            {
                Menu.ActiveMenu= Menu.ActiveMenuBck2;
                Menu.MenuSel= Menu.MenuSelBck2;
                GetMenuList();
                PrintScreen ();                
            }
        } // AUTO CYLINDER HEIGHT
*/
        
        
/*
        //----------------------------------------------------------------------
        // MANUAL CONTROL
        else if (Menu.ActiveMenu==mnManual)
        {
            if (Keys.Key2==Key_Enter)
            {
            }

            else if (Keys.Key2==Key_1)
            {
                VACUUM=!VACUUM;
                PrintScreen();
                while (Keys.KeyPressed);
            }
            else if (Keys.Key2==Key_2)
            {
                HORN=!HORN;
                PrintScreen();
                while (Keys.KeyPressed);
            }
            else if (Keys.Key2==Key_3)
            {
                TUBE_STOP=!TUBE_STOP;
                PrintScreen();
                while (Keys.KeyPressed);
            }
            else if (Keys.Key2==Key_4)
            {
                LIFT=!LIFT;
                PrintScreen();
                while (Keys.KeyPressed);
            }
            else if (Keys.Key2==Key_5)
            {
                HEAD_DOWN=!HEAD_DOWN;
                PrintScreen();
                while (Keys.KeyPressed);
            }
            else if (Keys.Key2==Key_6)
            {
                CLUTCH=!CLUTCH;
                PrintScreen();
                while (Keys.KeyPressed);
            }
            else if (Keys.Key2==Key_7)
            {
                HEAD=!HEAD;
                PrintScreen();
                while (Keys.KeyPressed);
            }
            else if (Keys.Key2==Key_8)
            {
                TUBE_OUT=!TUBE_OUT;
                PrintScreen();
                while (Keys.KeyPressed);
            }
            else if (Keys.Key2==Key_Clr)
            {
                LIFT= 0;
                //STOPPER= 0;
                Menu.ActiveMenu= Menu.ActiveMenuBck2;
                Menu.MenuSel= Menu.MenuSelBck2;
                GetMenuList();
                PrintScreen ();                
            }
        } // MANUAL CONTROL
      */
    } // if (MachineState== st_MENU) {

    
    ////////////////////////////////////////////////////////////////////////////////////7
    //  Hata i?lemleri
    //
    if (Parameters.MachineState== st_ERROR) 
    {
        if ((Keys.Key2==Key_PWR)||(Keys.Key2==Key_SET))
        {
            __delay_ms(100);        
            Parameters.MachineState= st_HEATER_WAIT;          
            Timers.tick_refreshdisp=0;
            Edit.Edit= 0;
            PrintScreen ();         
            System.CylSensorState= 0;
            System.HEATER_Chk= 0;
                HEATER= 0;
                __delay_ms (250);        
                Timers.tick_heater_chk=20;
                System.HEATER_Chk= 0;
                PRESSURE_RL= 0;
                __delay_ms (250);       
                System.PressureOk=0;
                Parameters.HeaterState= hs_HEATER_OFF;
                    if (!MANUAL) ALARM_AUTO= 0;
                        else 
                        {
                        ALARM_MAN= 0;
                        OUT4=0;
                        }
            
        } // if (MachineState== st_ERROR) {        
    }
    
}