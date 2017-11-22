    private void press_Back()
    {
        isPressOK=device.pressBack();
        System.out.println("BACK");
        //System.out.println("BACK" + "=" + isPressOK);
    }
    
    private void press_Home()
    {
        isPressOK=device.pressHome();
        System.out.println("HOME");
        //System.out.println("HOME" + "=" + isPressOK);
    }
    
    private void press_Menu()
    {
        isPressOK=device.pressMenu();
        System.out.println("MENU");
        //System.out.println("MENU" + "=" + isPressOK);
    }
    
    private void press_Left()
    {
        isPressOK=device.pressDPadLeft();
        System.out.println("LEFT" + "=" + isPressOK);
    }
    
    private void press_Right()
    {
        isPressOK=device.pressDPadRight();
        System.out.println("RIGHT" + "=" + isPressOK);
    }
    
    private void press_Up()
    {
        isPressOK=device.pressDPadUp();
        System.out.println("UP" + "=" + isPressOK);
    }
    
    private void press_Down()
    {
        isPressOK=device.pressDPadDown();
        System.out.println("DOWN" + "=" + isPressOK);
    }
    
    private void press_Enter()
    {
        isPressOK=device.pressEnter();
        System.out.println("ENTER" + "=" + isPressOK);
    }
    
    private void press_CenterCode(int code)
    {
        isPressOK=device.pressKeyCode(code);
        System.out.println("CenterCode" + "=" + isPressOK);
    }
    
    private void press_Power(int code)
    {
        isPressOK=device.pressKeyCode(code);
        System.out.println("Power" + "=" + isPressOK);
    }
    
    private void press_VolumeUp(int code)
    {
        isPressOK=device.pressKeyCode(code);
        System.out.println("VolumeUp" + "=" + isPressOK);
    }
    
    private void press_VolumeDown(int code)
    {
        isPressOK=device.pressKeyCode(code);
        System.out.println("VolumeDown" + "=" + isPressOK);
    }
    
    
    
    
    private void clickText_notExit(String arg_Text)
    {
        isXXX=true;
        clickText(arg_Text);
    }
    private void clickText_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        clickText(arg_Text,arg_index);
    }
    
    private void clickText(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickText_notExit " + arg_Text);
            }
            else
            {
                outPutScript("clickText " + arg_Text);
            }
            
            if(arg_Text.equals(strHot))
            {
                selector=getResourceIdSelector(hotResourceId);
                obj=getObj(selector);
            }
            else
            {
                selector=getTextSelector(arg_Text);
                obj=getObj(selector);
            }
            isClickOK=obj.click();
            /*
            rectxy=obj.getBounds();
            isClickOK=device.click(rectxy.centerX(),rectxy.centerY());
            
            if(arg_Text.equals(strConfirm) || arg_Text.equals(strCancel))
            {
                isClickOK=true;
            }
            */
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;
            if(isXXX)
            {
                outPutScript("clickText_notExit " + arg_Text + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickText " + arg_Text + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickText_notExit " + arg_Text + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickText " + arg_Text + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    private void clickText(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickText_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickText " + arg_Text + "    " + String.valueOf(arg_index));
            }
            if(arg_Text.equals(strHot))
            {
                selector=getResourceIdSelector(hotResourceId);
                obj=getObj(selector);
            }
            else
            {
                selector=getTextSelector(arg_Text,arg_index);
                obj=getObj(selector);
            }
            isClickOK=obj.click();
            //rectxy=obj.getBounds();
            //isClickOK=device.click(rectxy.centerX(),rectxy.centerY());
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickText_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickText " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickText_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickText " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    
    private void clickTextMatches_notExit(String arg_Text)
    {
        isXXX=true;
        clickTextMatches(arg_Text);
    }
    private void clickTextMatches_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        clickTextMatches(arg_Text,arg_index);
    }
    private void clickTextMatches(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickTextMatches_notExit " + arg_Text);
            }
            else
            {
                outPutScript("clickTextMatches " + arg_Text);
            }
            selector=getTextMatchesSelector(arg_Text);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickTextMatches_notExit " + arg_Text + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickTextMatches " + arg_Text + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickTextMatches_notExit " + arg_Text + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickTextMatches " + arg_Text + "    princekin" + isClickOK);
        }
        //return isClickOK;      
    }
    
    private void clickTextMatches(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickTextMatches_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickTextMatches " + arg_Text + "    " + String.valueOf(arg_index));
            }
            selector=getTextMatchesSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickTextMatches_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickTextMatches " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickTextMatches_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickTextMatches " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;      
    }
    
    
    
    private void clickTextStartWith_notExit(String arg_Text)
    {
        isXXX=true;
        clickTextStartWith(arg_Text);
    }
    private void clickTextStartWith_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        clickTextStartWith(arg_Text,arg_index);
    }
    private void clickTextStartWith(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickTextStartWith_notExit " + arg_Text);
            }
            else
            {
                outPutScript("clickTextStartWith " + arg_Text);
            }
            selector=getTextStartsWithSelector(arg_Text);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickTextStartWith_notExit " + arg_Text + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickTextStartWith " + arg_Text + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickTextStartWith_notExit " + arg_Text + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickTextStartWith " + arg_Text + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    private void clickTextStartWith(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickTextStartWith_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickTextStartWith " + arg_Text + "    " + String.valueOf(arg_index));
            }
            selector=getTextStartsWithSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickTextStartWith_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickTextStartWith " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickTextStartWith_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickTextStartWith " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    
    

    private void clickTextContains_notExit(String arg_Text)
    {
        isXXX=true;
        clickTextContains(arg_Text);
    }
    private void clickTextContains_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        clickTextContains(arg_Text,arg_index);
    }
    private void clickTextContains(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickTextContains_notExit " + arg_Text);
            }
            else
            {
                outPutScript("clickTextContains " + arg_Text);
            }
            selector=getTextContainsSelector(arg_Text);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickTextContains_notExit " + arg_Text + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickTextContains " + arg_Text + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickTextContains_notExit " + arg_Text + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickTextContains " + arg_Text + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    private void clickTextContains(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickTextContains_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickTextContains " + arg_Text + "    " + String.valueOf(arg_index));
            }
            selector=getTextContainsSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickTextContains_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickTextContains " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickTextContains_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickTextContains " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    
    
    
    
    
    private void clickDescription_notExit(String arg_Text)
    {
        isXXX=true;
        clickDescription(arg_Text);
    }
    private void clickDescription_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        clickDescription(arg_Text,arg_index);
    }
    private void clickDescription(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickDescription_notExit " + arg_Text);
            }
            else
            {
                outPutScript("clickDescription " + arg_Text);
            }

            selector=getDescriptionSelector(arg_Text);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickDescription_notExit " + arg_Text + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickDescription " + arg_Text + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickDescription_notExit " + arg_Text + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickDescription " + arg_Text + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    private void clickDescription(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickDescription_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickDescription " + arg_Text + "    " + String.valueOf(arg_index));
            }
            selector=getDescriptionSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickDescription_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickDescription " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickDescription_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickDescription " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    
    
    

    private void clickDescriptionMatches_notExit(String arg_Text)
    {
        isXXX=true;
        clickDescriptionMatches(arg_Text);
    }
    private void clickDescriptionMatches_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        clickDescriptionMatches(arg_Text,arg_index);
    }
    private void clickDescriptionMatches(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickDescriptionMatches_notExit " + arg_Text);
            }
            else
            {
                outPutScript("clickDescriptionMatches " + arg_Text);
            }
            selector=getDescriptionMatchesSelector(arg_Text);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickDescriptionMatches_notExit " + arg_Text + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickDescriptionMatches " + arg_Text + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickDescriptionMatches_notExit " + arg_Text + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickDescriptionMatches " + arg_Text + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    private void clickDescriptionMatches(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickDescriptionMatches_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickDescriptionMatches " + arg_Text + "    " + String.valueOf(arg_index));
            }
            selector=getDescriptionMatchesSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickDescriptionMatches_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickDescriptionMatches " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickDescriptionMatches_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickDescriptionMatches " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    
    
    
    

    private void clickDescriptionStartWith_notExit(String arg_Text)
    {
        isXXX=true;
        clickDescriptionStartWith(arg_Text);
    }
    private void clickDescriptionStartWith_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        clickDescriptionStartWith(arg_Text,arg_index);
    }
    private void clickDescriptionStartWith(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickDescriptionStartWith_notExit " + arg_Text);
            }
            else
            {
                outPutScript("clickDescriptionStartWith " + arg_Text);
            }
            selector=getDescriptionStartsWithSelector(arg_Text);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickDescriptionStartWith_notExit " + arg_Text + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickDescriptionStartWith " + arg_Text + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickDescriptionStartWith_notExit " + arg_Text + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickDescriptionStartWith " + arg_Text + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    private void clickDescriptionStartWith(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickDescriptionStartWith_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickDescriptionStartWith " + arg_Text + "    " + String.valueOf(arg_index));
            }
            selector=getDescriptionStartsWithSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickDescriptionStartWith_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickDescriptionStartWith " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickDescriptionStartWith_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickDescriptionStartWith " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    
    

    
    private void clickDescriptionContains_notExit(String arg_Text)
    {
        isXXX=true;
        clickDescriptionContains(arg_Text);
    }
    private void clickDescriptionContains_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        clickDescriptionContains(arg_Text,arg_index);
    }
    private void clickDescriptionContains(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickDescriptionContains_notExit " + arg_Text);
            }
            else
            {
                outPutScript("clickDescriptionContains " + arg_Text);
            }
            selector=getDescriptionContainsSelector(arg_Text);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickDescriptionContains_notExit " + arg_Text + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickDescriptionContains " + arg_Text + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickDescriptionContains_notExit " + arg_Text + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickDescriptionContains " + arg_Text + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    private void clickDescriptionContains(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickDescriptionContains_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickDescriptionContains " + arg_Text + "    " + String.valueOf(arg_index));
            }
            selector=getDescriptionContainsSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickDescriptionContains_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickDescriptionContains " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickDescriptionContains_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickDescriptionContains " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    
    
    
    private void clickResourceId_notExit(String arg_Text)
    {
        isXXX=true;
        clickResourceId(arg_Text);
    }
    private void clickResourceId_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        clickResourceId(arg_Text,arg_index);
    }
    private void clickResourceId(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickResourceId_notExit " + arg_Text);
            }
            else
            {
                outPutScript("clickResourceId " + arg_Text);
            }
            selector=getResourceIdSelector(arg_Text);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickResourceId_notExit " + arg_Text + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickResourceId " + arg_Text + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickResourceId_notExit " + arg_Text + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickResourceId " + arg_Text + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    private void clickResourceId(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickResourceId_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickResourceId " + arg_Text + "    " + String.valueOf(arg_index));
            }
            selector=getResourceIdSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickResourceId_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickResourceId " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickResourceId_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickResourceId " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    
    
    
    
    
    private void clickResourceIdMatches_notExit(String arg_Text)
    {
        isXXX=true;
        clickResourceIdMatches(arg_Text);
    }
    private void clickResourceIdMatches_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        clickResourceIdMatches(arg_Text,arg_index);
    }
    private void clickResourceIdMatches(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickResourceIdMatches_notExit " + arg_Text);
            }
            else
            {
                outPutScript("clickResourceIdMatches " + arg_Text);
            }
            selector=getResourceIdMatchesSelector(arg_Text);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickResourceIdMatches_notExit " + arg_Text + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickResourceIdMatches " + arg_Text + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickResourceIdMatches_notExit " + arg_Text + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickResourceIdMatches " + arg_Text + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    private void clickResourceIdMatches(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickResourceIdMatches_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickResourceIdMatches " + arg_Text + "    " + String.valueOf(arg_index));
            }
            selector=getResourceIdMatchesSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickResourceIdMatches_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickResourceIdMatches " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickResourceIdMatches_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickResourceIdMatches " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    
    
    
    

    private void clickClassName_notExit(String arg_Text)
    {
        isXXX=true;
        clickClassName(arg_Text);
    }
    private void clickClassName_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        clickClassName(arg_Text,arg_index);
    }
    private void clickClassName(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickClassName_notExit " + arg_Text);
            }
            else
            {
                outPutScript("clickClassName " + arg_Text);
            }
            selector=getClassNameSelector(arg_Text);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickClassName_notExit " + arg_Text + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickClassName " + arg_Text + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickClassName_notExit " + arg_Text + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickClassName " + arg_Text + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    private void clickClassName(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickClassName_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickClassName " + arg_Text + "    " + String.valueOf(arg_index));
            }
            selector=getClassNameSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickClassName_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickClassName " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickClassName_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickClassName " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    
    
    
    
    private void clickClassNameMatches_notExit(String arg_Text)
    {
        isXXX=true;
        clickClassNameMatches(arg_Text);
    }
    private void clickClassNameMatches_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        clickClassNameMatches(arg_Text,arg_index);
    }
    private void clickClassNameMatches(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickClassNameMatches_notExit " + arg_Text);
            }
            else
            {
                outPutScript("clickClassNameMatches " + arg_Text);
            }
            selector=getClassNameMatchesSelector(arg_Text);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickClassNameMatches_notExit " + arg_Text + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickClassNameMatches " + arg_Text + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickClassNameMatches_notExit " + arg_Text + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickClassNameMatches " + arg_Text + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    private void clickClassNameMatches(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickClassNameMatches_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickClassNameMatches " + arg_Text + "    " + String.valueOf(arg_index));
            }
            selector=getClassNameMatchesSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isClickOK=obj.click();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickClassNameMatches_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickClassNameMatches " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickClassNameMatches_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickClassNameMatches " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    
    
    
    private void clickFocused_notExit(boolean arg_bool)
    {
        isXXX=true;
        clickFocused(arg_bool);
    }
    private void clickFocused_notExit(boolean arg_bool,int arg_index)
    {
        isXXX=true;
        clickFocused(arg_bool,arg_index);
    }
    private void clickFocused(boolean arg_bool)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickFocused_notExit " + arg_bool);
            }
            else
            {
                outPutScript("clickFocused " + arg_bool);
            }
            selector=getFocusedSelector(arg_bool);
            obj=getObj(selector);
            isClickOK=true;
            //isClickOK=obj.click();
        }
        catch(Exception e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickFocused_notExit " + arg_bool + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickFocused " + arg_bool + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickFocused_notExit " + arg_bool + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickFocused " + arg_bool + "    princekin" + isClickOK);
        }
    }
    
    private void clickFocused(boolean arg_bool,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickFocused_notExit " + arg_bool  + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickFocused " + arg_bool  + "    " + String.valueOf(arg_index));
            }
            selector=getFocusedSelector(arg_bool,arg_index);
            obj=getObj(selector);
            isClickOK=true;
            //isClickOK=obj.click();
        }
        catch(Exception e)
        {
        	  isClickOK=false;        	
            if(isXXX)
            {
                outPutScript("clickFocused_notExit " + arg_bool + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickFocused " + arg_bool + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickFocused_notExit " + arg_bool + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickFocused " + arg_bool + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
    }
    
    
    
    
    
    
    private void clickFocusable_notExit(boolean arg_bool)
    {
        isXXX=true;
        clickFocusable(arg_bool);
    }
    private void clickFocusable_notExit(boolean arg_bool,int arg_index)
    {
        isXXX=true;
        clickFocusable(arg_bool,arg_index);
    }
    private void clickFocusable(boolean arg_bool)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickFocusable_notExit " + arg_bool);
            }
            else
            {
                outPutScript("clickFocusable " + arg_bool);
            }
            selector=getFocusableSelector(arg_bool);
            obj=getObj(selector);
            isClickOK=true;
            //isClickOK=obj.click();
        }
        catch(Exception e)
        {
        	  isClickOK=false;
            if(isXXX)
            {
                outPutScript("clickFocusable_notExit " + arg_bool + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickFocusable " + arg_bool + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickFocusable_notExit " + arg_bool + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickFocusable " + arg_bool + "    princekin" + isClickOK);
        }
    }
    
    private void clickFocusable(boolean arg_bool,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clickFocusable_notExit " + arg_bool  + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("clickFocusable " + arg_bool  + "    " + String.valueOf(arg_index));
            }
            selector=getFocusableSelector(arg_bool,arg_index);
            obj=getObj(selector);
            isClickOK=true;
            //isClickOK=obj.click();
        }
        catch(Exception e)
        {
        	  isClickOK=false;
            if(isXXX)
            {
                outPutScript("clickFocusable_notExit " + arg_bool + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("clickFocusable " + arg_bool + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clickFocusable_notExit " + arg_bool + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("clickFocusable " + arg_bool + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
    }
    
    
    
    
    
    
    
    private void checkText_notExit(String arg_Text)
    {
        isXXX=true;
        checkText(arg_Text);
    }
    private void checkText_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        checkText(arg_Text,arg_index);
    }
    private void checkText(String arg_Text)
    {
        selector=getTextSelector(arg_Text);
        obj=getObj(selector);
        isCheckOK=obj.exists();
        if(isXXX)
        {
            isXXX=false;
            outPutScript("checkText_notExit " + arg_Text + "    princekin_notExit" + isCheckOK);
        }
        else
        {
            outPutScript("checkText " + arg_Text + "    princekin" + isCheckOK);
        }
        //return isCheckOK;  
    }
    private void checkText(String arg_Text,int arg_index)
    {
        selector=getTextSelector(arg_Text,arg_index);
        obj=getObj(selector);
        isCheckOK=obj.exists();
        if(isXXX)
        {
            isXXX=false;
            outPutScript("checkText_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isCheckOK);
        }
        else
        {
            outPutScript("checkText " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isCheckOK);
        }
        //return isCheckOK;  
    }
    
    
    
    
    private void checkTextMatches_notExit(String arg_Text)
    {
        isXXX=true;
        checkTextMatches(arg_Text);
    }
    private void checkTextMatches_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        checkTextMatches(arg_Text,arg_index);
    }
    private void checkTextMatches(String arg_Text)
    {
        selector=getTextMatchesSelector(arg_Text);
        obj=getObj(selector);
        isCheckOK=obj.exists();
        if(isXXX)
        {
            isXXX=false;
            outPutScript("checkTextMatches_notExit " + arg_Text + "    princekin_notExit" + isCheckOK);
        }
        else
        {
            outPutScript("checkTextMatches " + arg_Text + "    princekin" + isCheckOK);
        }
        //return isCheckOK;
    }
    private void checkTextMatches(String arg_Text,int arg_index)
    {
        selector=getTextMatchesSelector(arg_Text,arg_index);
        obj=getObj(selector);
        isCheckOK=obj.exists();
        if(isXXX)
        {
            isXXX=false;
            outPutScript("checkTextMatches_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isCheckOK);
        }
        else
        {
            outPutScript("checkTextMatches " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isCheckOK);
        }
        //return isCheckOK;
    }
    
    
    
    

    private void checkTextStartWith_notExit(String arg_Text)
    {
        isXXX=true;
        checkTextStartWith(arg_Text);
    }
    private void checkTextStartWith_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        checkTextStartWith(arg_Text,arg_index);
    }
    private void checkTextStartWith(String arg_Text)
    {
        selector=getTextStartsWithSelector(arg_Text);
        obj=getObj(selector);
        isCheckOK=obj.exists();
        if(isXXX)
        {
            isXXX=false;
            outPutScript("checkTextStartWith_notExit " + arg_Text + "    princekin_notExit" + isCheckOK);
        }
        else
        {
            outPutScript("checkTextStartWith " + arg_Text + "    princekin" + isCheckOK);
        }
        //return isCheckOK;
    }
    private void checkTextStartWith(String arg_Text,int arg_index)
    {
        selector=getTextStartsWithSelector(arg_Text,arg_index);
        obj=getObj(selector);
        isCheckOK=obj.exists();
        if(isXXX)
        {
            isXXX=false;
            outPutScript("checkTextStartWith_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isCheckOK);
        }
        else
        {
            outPutScript("checkTextStartWith " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isCheckOK);
        }
        //return isCheckOK;
    }
    
    
    
    
    
    private void checkTextContains_notExit(String arg_Text)
    {
        isXXX=true;
        checkTextContains(arg_Text);
    }
    private void checkTextContains_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        checkTextContains(arg_Text,arg_index);
    }
    private void checkTextContains(String arg_Text)
    {
        selector=getTextContainsSelector(arg_Text);
        obj=getObj(selector);
        isCheckOK=obj.exists();
        if(isXXX)
        {
            isXXX=false;
            outPutScript("checkTextContains_notExit " + arg_Text + "    princekin_notExit" + isCheckOK);
        }
        else
        {
            outPutScript("checkTextContains " + arg_Text + "    princekin" + isCheckOK);
        }
        //return isCheckOK; 
    }
    private void checkTextContains(String arg_Text,int arg_index)
    {
        selector=getTextContainsSelector(arg_Text,arg_index);
        obj=getObj(selector);
        isCheckOK=obj.exists();
        if(isXXX)
        {
            isXXX=false;
            outPutScript("checkTextContains_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isCheckOK);
        }
        else
        {
            outPutScript("checkTextContains " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isCheckOK);
        }
        //return isCheckOK; 
    }
    
    
    

    
    
    private void checkTextSelected_notExit(String arg_Text)
    {
        isXXX=true;
        checkTextSelected(arg_Text);
    }
    private void checkTextSelected_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        checkTextSelected(arg_Text,arg_index);
    }
    private void checkTextSelected(String arg_Text)
    {
        try
        {
            selector=getTextSelector(arg_Text);
            obj=getObj(selector);
            isCheckOK=obj.isSelected();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isCheckOK=false;
            if(isXXX)
            {
                outPutScript("checkTextSelected_notExit " + arg_Text + "    princekin_notExit" + isCheckOK + "    NotFound");
            }
            else
            {
                outPutScript("checkTextSelected " + arg_Text + "    princekin" + isCheckOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("checkTextSelected_notExit " + arg_Text + "    princekin_notExit" + isCheckOK);
        }
        else
        {
            outPutScript("checkTextSelected " + arg_Text + "    princekin" + isCheckOK);
        }
        //return isCheckOK; 
    }
    private void checkTextSelected(String arg_Text,int arg_index)
    {
        try
        {
            selector=getTextSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isCheckOK=obj.isSelected();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isCheckOK=false;
            if(isXXX)
            {
                outPutScript("checkTextSelected_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isCheckOK + "    NotFound");
            }
            else
            {
                outPutScript("checkTextSelected " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isCheckOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("checkTextSelected_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isCheckOK);
        }
        else
        {
            outPutScript("checkTextSelected " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isCheckOK);
        }
        //return isCheckOK; 
    }
    
    
    
    
    
    private void checkTextChecked_notExit(String arg_Text)
    {
        isXXX=true;
        checkTextChecked(arg_Text);
    }
    private void checkTextChecked_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        checkTextChecked(arg_Text,arg_index);
    }
    private void checkTextChecked(String arg_Text)
    {
        try
        {
            selector=getTextSelector(arg_Text);
            obj=getObj(selector);
            isCheckOK=obj.isChecked();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isCheckOK=false;
            if(isXXX)
            {
                outPutScript("checkTextChecked_notExit " + arg_Text + "    princekin_notExit" + isCheckOK + "    NotFound");
            }
            else
            {
                outPutScript("checkTextChecked " + arg_Text + "    princekin" + isCheckOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("checkTextChecked_notExit " + arg_Text + "    princekin_notExit" + isCheckOK);
        }
        else
        {
            outPutScript("checkTextChecked " + arg_Text + "    princekin" + isCheckOK);
        }
        //return isCheckOK;
    }
    private void checkTextChecked(String arg_Text,int arg_index)
    {
        try
        {
            selector=getTextSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isCheckOK=obj.isChecked();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isCheckOK=false;
            if(isXXX)
            {
                outPutScript("checkTextChecked_notExit " + arg_Text + "    princekin_notExit" + isCheckOK + "    NotFound");
            }
            else
            {
                outPutScript("checkTextChecked " + arg_Text + "    princekin" + isCheckOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("checkTextChecked_notExit " + arg_Text + "    princekin_notExit" + isCheckOK);
        }
        else
        {
            outPutScript("checkTextChecked " + arg_Text + "    princekin" + isCheckOK);
        }
        //return isCheckOK;
    }
    
    
    
    private UiSelector getFocusedSelector(boolean arg_bool)
    {
        UiSelector mSelector=new UiSelector().focused(arg_bool);
        return mSelector;
    }
    private UiSelector getFocusedSelector(boolean arg_bool,int arg_index)
    {
        UiSelector mSelector=new UiSelector().focused(arg_bool).instance(arg_index);
        return mSelector;
    }
    
    private UiSelector getFocusableSelector(boolean arg_bool)
    {
        UiSelector mSelector=new UiSelector().focusable(arg_bool);
        return mSelector;
    }
    private UiSelector getFocusableSelector(boolean arg_bool,int arg_index)
    {
        UiSelector mSelector=new UiSelector().focusable(arg_bool).instance(arg_index);
        return mSelector;
    }
    
    
            
    private UiSelector getTextSelector(String arg_Text)
    {
        UiSelector mSelector=new UiSelector().text(arg_Text);
        return mSelector;
    }
    private UiSelector getTextSelector(String arg_Text,int arg_index)
    {
        UiSelector mSelector=new UiSelector().text(arg_Text).instance(arg_index);
        return mSelector;
    }
    
    
    private UiSelector getTextMatchesSelector(String arg_Text)
    {
        UiSelector mSelector=new UiSelector().textMatches(arg_Text);
        return mSelector;
    }
    private UiSelector getTextMatchesSelector(String arg_Text,int arg_index)
    {
        UiSelector mSelector=new UiSelector().textMatches(arg_Text).instance(arg_index);
        return mSelector;
    }
    
    
    
    
    private UiSelector getTextStartsWithSelector(String arg_Text)
    {
        UiSelector mSelector=new UiSelector().textStartsWith(arg_Text);
        return mSelector;
    }
    private UiSelector getTextStartsWithSelector(String arg_Text,int arg_index)
    {
        UiSelector mSelector=new UiSelector().textStartsWith(arg_Text).instance(arg_index);
        return mSelector;
    }
    
    
    
    
    
    private UiSelector getTextContainsSelector(String arg_Text)
    {
        UiSelector mSelector=new UiSelector().textContains(arg_Text);
        return mSelector;
    }
    private UiSelector getTextContainsSelector(String arg_Text,int arg_index)
    {
        UiSelector mSelector=new UiSelector().textContains(arg_Text).instance(arg_index);
        return mSelector;
    }
    
    
    
    private UiSelector getDescriptionSelector(String arg_Text)
    {
        UiSelector mSelector=new UiSelector().description(arg_Text);
        return mSelector;
    }
    private UiSelector getDescriptionSelector(String arg_Text,int arg_index)
    {
        UiSelector mSelector=new UiSelector().description(arg_Text).instance(arg_index);
        return mSelector;
    }
    
    
    
    private UiSelector getDescriptionMatchesSelector(String arg_Text)
    {
        UiSelector mSelector=new UiSelector().descriptionMatches(arg_Text);
        return mSelector;
    }
    private UiSelector getDescriptionMatchesSelector(String arg_Text,int arg_index)
    {
        UiSelector mSelector=new UiSelector().descriptionMatches(arg_Text).instance(arg_index);
        return mSelector;
    }
    
    
    
    private UiSelector getDescriptionStartsWithSelector(String arg_Text)
    {
        UiSelector mSelector=new UiSelector().descriptionStartsWith(arg_Text);
        return mSelector;
    }
    private UiSelector getDescriptionStartsWithSelector(String arg_Text,int arg_index)
    {
        UiSelector mSelector=new UiSelector().descriptionStartsWith(arg_Text).instance(arg_index);
        return mSelector;
    }
    
    
    
    
    private UiSelector getDescriptionContainsSelector(String arg_Text)
    {
        UiSelector mSelector=new UiSelector().descriptionContains(arg_Text);
        return mSelector;
    }
    private UiSelector getDescriptionContainsSelector(String arg_Text,int arg_index)
    {
        UiSelector mSelector=new UiSelector().descriptionContains(arg_Text).instance(arg_index);
        return mSelector;
    }
    
    
    
    
    private UiSelector getResourceIdSelector(String arg_Text)
    {
        UiSelector mSelector=new UiSelector().resourceId(arg_Text);
        return mSelector;
    }
    private UiSelector getResourceIdSelector(String arg_Text,int arg_index)
    {
        UiSelector mSelector=new UiSelector().resourceId(arg_Text).instance(arg_index);
        return mSelector;
    }
    
    
    
    
    
    
    private UiSelector getResourceIdMatchesSelector(String arg_Text)
    {
        UiSelector mSelector=new UiSelector().resourceIdMatches(arg_Text);
        return mSelector;
    }
    private UiSelector getResourceIdMatchesSelector(String arg_Text,int arg_index)
    {
        UiSelector mSelector=new UiSelector().resourceIdMatches(arg_Text).instance(arg_index);
        return mSelector;
    }
    
    
    
    
    private UiSelector getClassNameSelector(String arg_Text)
    {
        UiSelector mSelector=new UiSelector().className(arg_Text);
        return mSelector;
    }
    private UiSelector getClassNameSelector(String arg_Text,int arg_index)
    {
        UiSelector mSelector=new UiSelector().className(arg_Text).instance(arg_index);
        return mSelector;
    }
    
    
    
    
    private UiSelector getClassNameMatchesSelector(String arg_Text)
    {
        UiSelector mSelector=new UiSelector().classNameMatches(arg_Text);
        return mSelector;
    }
    private UiSelector getClassNameMatchesSelector(String arg_Text,int arg_index)
    {
        UiSelector mSelector=new UiSelector().classNameMatches(arg_Text).instance(arg_index);
        return mSelector;
    }
    
    
    
    private UiSelector getScrollableSelector(boolean arg_bool)
    {
        UiSelector mSelector=new UiSelector().scrollable(arg_bool);
        return mSelector;
    }
    private UiSelector getScrollableSelector(boolean arg_bool,int arg_index)
    {
        UiSelector mSelector=new UiSelector().scrollable(arg_bool).instance(arg_index);
        return mSelector;
    }
    //******************************************************************************************//
    
    
    
    
    
    //******************************************************************************************//
    private void locateClassName(String arg_Text)
    {
        selector=getClassNameSelector(arg_Text);
        setScrollInstance(selector);
    }
    private void locateClassName(String arg_Text,int arg_index)
    {
        selector=getClassNameSelector(arg_Text,arg_index);
        setScrollInstance(selector);
    }
    
    
    
    private void locateClassNameMatches(String arg_Text)
    {
        selector=getClassNameMatchesSelector(arg_Text);
        setScrollInstance(selector);
    }
    private void locateClassNameMatches(String arg_Text,int arg_index)
    {
        selector=getClassNameMatchesSelector(arg_Text,arg_index);
        setScrollInstance(selector);
    }
    
    
    
    
    
    private void locateScrollable(boolean arg_bool)
    {
        selector=getScrollableSelector(arg_bool);
        setScrollInstance(selector);
    }
    private void locateScrollable(boolean arg_bool,int arg_index)
    {
        selector=getScrollableSelector(arg_bool,arg_index);
        setScrollInstance(selector);
    }
    
    private void scrollId_notExit(String arg_Text)
    {
        isXXX=true;
        scrollId(arg_Text);
    }
    private void scrollId_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        scrollId(arg_Text,arg_index);
    }
    private void scrollId(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollId_notExit " + arg_Text);
            }
            else
            {
                outPutScript("scrollId " + arg_Text);
            }
            selector=getResourceIdSelector(arg_Text);
            obj=getObj(selector);
            isScrollOK=scroll.scrollIntoView(obj);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollId_notExit " + arg_Text + "    princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollId " + arg_Text + "    princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollId_notExit " + arg_Text + "    princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("scrollId " + arg_Text + "    princekin" + isScrollOK);
        }
        //return isScrollOK;
    }
    private void scrollId(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollId_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("scrollId " + arg_Text + "    " + String.valueOf(arg_index));
            }
            selector=getResourceIdSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isScrollOK=scroll.scrollIntoView(obj);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollId_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollId " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollId_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("scrollId " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    
    
    
    
    
    
    
    
    private void scrollClass_notExit(String arg_Text)
    {
        isXXX=true;
        scrollClass(arg_Text);
    }
    private void scrollClass_notExit(String arg_Text,int arg_index)
    {
        isXXX=true;
        scrollClass(arg_Text,arg_index);
    }
    private void scrollClass(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollClass_notExit " + arg_Text);
            }
            else
            {
                outPutScript("scrollClass " + arg_Text);
            }
            selector=getClassNameSelector(arg_Text);
            obj=getObj(selector);
            isScrollOK=scroll.scrollIntoView(obj);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollClass_notExit " + arg_Text + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollClass " + arg_Text + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollClass_notExit " + arg_Text + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("scrollClass " + arg_Text + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    private void scrollClass(String arg_Text,int arg_index)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollClass_notExit " + arg_Text + "    " + String.valueOf(arg_index));
            }
            else
            {
                outPutScript("scrollClass " + arg_Text + "    " + String.valueOf(arg_index));
            }
            selector=getClassNameSelector(arg_Text,arg_index);
            obj=getObj(selector);
            isScrollOK=scroll.scrollIntoView(obj);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollClass_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollClass " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollClass_notExit " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin_notExit" + isClickOK);
        }
        else
        {
            outPutScript("scrollClass " + arg_Text + "    " + String.valueOf(arg_index) + "    princekin" + isClickOK);
        }
        //return isClickOK;
    }
    
    private void scrollTextIntoView_notExit(String arg_Text)
    {
        isXXX=true;
        scrollTextIntoView(arg_Text);
    }
    private void scrollTextIntoView(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollTextIntoView_notExit " + arg_Text);
            }
            else
            {
                outPutScript("scrollTextIntoView " + arg_Text);
            }
            isScrollOK=scroll.scrollTextIntoView(arg_Text);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollTextIntoView_notExit " + arg_Text + "    princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollTextIntoView " + arg_Text + "    princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollTextIntoView_notExit " + arg_Text + "    princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("scrollTextIntoView " + arg_Text + "    princekin" + isScrollOK);
        }
        //return isScrollOK;
    }
    
    
    private void scrollDescriptionIntoView_notExit(String arg_Text)
    {
        isXXX=true;
        scrollDescriptionIntoView(arg_Text);
    }
    private void scrollDescriptionIntoView(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollDescriptionIntoView_notExit " + arg_Text);
            }
            else
            {
                outPutScript("scrollDescriptionIntoView " + arg_Text);
            }
            isScrollOK=scroll.scrollDescriptionIntoView(arg_Text);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollDescriptionIntoView_notExit " + arg_Text + "    princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollDescriptionIntoView " + arg_Text + "    princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollDescriptionIntoView_notExit " + arg_Text + "    princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("scrollDescriptionIntoView " + arg_Text + "    princekin" + isScrollOK);
        }
        //return isScrollOK;
    }
    
    
    
    private void flingForward_notExit()
    {
        isXXX=true;
        flingForward();
    }
    private void flingForward()
    {
        try
        {
            if(isXXX)
            {
                outPutScript("flingForward_notExit");
            }
            else
            {
                outPutScript("flingForward");
            }
            isScrollOK=scroll.flingForward();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("flingForward_notExit" + "     princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("flingForward" + "     princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("flingForward_notExit" + "     princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("flingForward" + "     princekin" + isScrollOK);
        }
        //return isScrollOK;
    }
    
    
    
    
    private void flingBackward_notExit()
    {
        isXXX=true;
        flingBackward();
    }
    private void flingBackward()
    {
        try
        {
            if(isXXX)
            {
                outPutScript("flingBackward_notExit");
            }
            else
            {
                outPutScript("flingBackward");
            }
            isScrollOK=scroll.flingBackward();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("flingBackward_notExit" + "     princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("flingBackward" + "     princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("flingBackward_notExit" + "     princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("flingBackward" + "     princekin" + isScrollOK);
        }
        //return isScrollOK;
    }

    
    
    private void scrollForward_notExit()
    {
        isXXX=true;
        scrollForward();
    }
    private void scrollForward_notExit(int arg_steps)
    {
        isXXX=true;
        scrollForward(arg_steps);
    }
    private void scrollForward()
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollForward_notExit");
            }
            else
            {
                outPutScript("scrollForward");
            }
            isScrollOK=scroll.scrollForward();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollForward_notExit" + "     princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollForward" + "     princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollForward_notExit" + "     princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("scrollForward" + "     princekin" + isScrollOK);
        }
        //return isScrollOK;
    }
    private void scrollForward(int arg_steps)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollForward_notExit " + String.valueOf(arg_steps));
            }
            else
            {
                outPutScript("scrollForward " + String.valueOf(arg_steps));
            }
            isScrollOK=scroll.scrollForward(arg_steps);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollForward_notExit" + String.valueOf(arg_steps) + "     princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollForward" + String.valueOf(arg_steps) + "     princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollForward_notExit" + String.valueOf(arg_steps) + "     princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("scrollForward" + String.valueOf(arg_steps) + "     princekin" + isScrollOK);
        }
        //return isScrollOK;
    }
    
    

    private void scrollBackward_notExit()
    {
        isXXX=true;
        scrollBackward();
    }
    private void scrollBackward_notExit(int arg_steps)
    {
        isXXX=true;
        scrollBackward(arg_steps);
    }
    private void scrollBackward()
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollBackward_notExit");
            }
            else
            {
                outPutScript("scrollBackward");
            }
            isScrollOK=scroll.scrollBackward();
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollBackward_notExit" + "     princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollBackward" + "     princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollBackward_notExit" + "     princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("scrollBackward" + "     princekin" + isScrollOK);
        }
        //return isScrollOK;
    }
    private void scrollBackward(int arg_steps)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollBackward_notExit " + String.valueOf(arg_steps));
            }
            else
            {
                outPutScript("scrollBackward " + String.valueOf(arg_steps));
            }
            isScrollOK=scroll.scrollBackward(arg_steps);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollBackward_notExit" + String.valueOf(arg_steps) + "     princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollBackward" + String.valueOf(arg_steps) + "     princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollBackward_notExit" + String.valueOf(arg_steps) + "     princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("scrollBackward" + String.valueOf(arg_steps) + "     princekin" + isScrollOK);
        }
        //return isScrollOK;
    }

    
    
    
    private void flingToEnd_notExit(int arg_maxSwipes)
    {
        isXXX=true;
        flingToEnd(arg_maxSwipes);
    }
    private void flingToEnd(int arg_maxSwipes)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("flingToEnd_notExit " + String.valueOf(arg_maxSwipes));
            }
            else
            {
                outPutScript("flingToEnd " + String.valueOf(arg_maxSwipes));
            }
            isScrollOK=scroll.flingToEnd(arg_maxSwipes);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("flingToEnd_notExit " + String.valueOf(arg_maxSwipes) + "     princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("flingToEnd " + String.valueOf(arg_maxSwipes) + "     princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("flingToEnd_notExit " + String.valueOf(arg_maxSwipes) + "     princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("flingToEnd " + String.valueOf(arg_maxSwipes) + "     princekin" + isScrollOK);
        }
        //return isScrollOK;
    }
    
    

    private void flingToBeginning_notExit(int arg_maxSwipes)
    {
        isXXX=true;
        flingToBeginning(arg_maxSwipes);
    }
    private void flingToBeginning(int arg_maxSwipes)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("flingToBeginning_notExit " + String.valueOf(arg_maxSwipes));
            }
            else
            {
                outPutScript("flingToBeginning " + String.valueOf(arg_maxSwipes));
            }
            isScrollOK=scroll.flingToBeginning(arg_maxSwipes);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("flingToBeginning_notExit " + String.valueOf(arg_maxSwipes) + "     princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("flingToBeginning " + String.valueOf(arg_maxSwipes) + "     princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("flingToBeginning_notExit " + String.valueOf(arg_maxSwipes) + "     princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("flingToBeginning " + String.valueOf(arg_maxSwipes) + "     princekin" + isScrollOK);
        }
        //return isScrollOK;
    }
    
    
    
    
    
    
    
    //********************************** scrollToEnd **********************************//
    private void scrollToEnd_notExit(int arg_maxSwipes)
    {
        isXXX=true;
        scrollToEnd(arg_maxSwipes);
    }
    private void scrollToEnd2(int arg_maxSwipes,int arg_steps)
    {
        scrollToEnd(arg_maxSwipes,arg_steps);
    }
    private void scrollToEnd2_notExit(int arg_maxSwipes,int arg_steps)
    {
        isXXX=true;
        scrollToEnd(arg_maxSwipes,arg_steps);
    }
    private void scrollToEnd(int arg_maxSwipes)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollToEnd_notExit " + String.valueOf(arg_maxSwipes));
            }
            else
            {
                outPutScript("scrollToEnd " + String.valueOf(arg_maxSwipes));
            }
            isScrollOK=scroll.scrollToEnd(arg_maxSwipes);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollToEnd_notExit " + String.valueOf(arg_maxSwipes) + "     princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollToEnd " + String.valueOf(arg_maxSwipes) + "     princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollToEnd_notExit " + String.valueOf(arg_maxSwipes) + "     princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("scrollToEnd " + String.valueOf(arg_maxSwipes) + "     princekin" + isScrollOK);
        }
        //return isScrollOK;
    }
    private void scrollToEnd(int arg_maxSwipes,int arg_steps)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollToEnd2_notExit " + String.valueOf(arg_maxSwipes) + " " + String.valueOf(arg_steps));
            }
            else
            {
                outPutScript("scrollToEnd2 " + String.valueOf(arg_maxSwipes) + " " + String.valueOf(arg_steps));
            }
            isScrollOK=scroll.scrollToEnd(arg_maxSwipes,arg_steps);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollToEnd2_notExit " + String.valueOf(arg_maxSwipes) + " " + String.valueOf(arg_steps) + "     princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollToEnd2 " + String.valueOf(arg_maxSwipes) + " " + String.valueOf(arg_steps) + "     princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollToEnd2_notExit " + String.valueOf(arg_maxSwipes) + " " + String.valueOf(arg_steps) + "     princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("scrollToEnd2 " + String.valueOf(arg_maxSwipes) + " " + String.valueOf(arg_steps) + "     princekin" + isScrollOK);
        }
        //return isScrollOK;
    }
    //********************************** scrollToEnd **********************************//
    
    
    
    //********************************** scrollToBeginning **********************************//
    private void scrollToBeginning_notExit(int arg_maxSwipes)
    {
        isXXX=true;
        scrollToBeginning(arg_maxSwipes);
    }
    private void scrollToBeginning2(int arg_maxSwipes,int arg_steps)
    {
        scrollToBeginning(arg_maxSwipes,arg_steps);
    }
    private void scrollToBeginning2_notExit(int arg_maxSwipes,int arg_steps)
    {
        isXXX=true;
        scrollToBeginning(arg_maxSwipes,arg_steps);
    }
    private void scrollToBeginning(int arg_maxSwipes)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollToBeginning_notExit " + String.valueOf(arg_maxSwipes));
            }
            else
            {
                outPutScript("scrollToBeginning " + String.valueOf(arg_maxSwipes));
            }
            isScrollOK=scroll.scrollToBeginning(arg_maxSwipes);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollToBeginning_notExit " + String.valueOf(arg_maxSwipes) + "     princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollToBeginning " + String.valueOf(arg_maxSwipes) + "     princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollToBeginning_notExit " + String.valueOf(arg_maxSwipes) + "     princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("scrollToBeginning " + String.valueOf(arg_maxSwipes) + "     princekin" + isScrollOK);
        }
        //return isScrollOK;
    }
    private void scrollToBeginning(int arg_maxSwipes,int arg_steps)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("scrollToBeginning2_notExit " + String.valueOf(arg_maxSwipes) + " " + String.valueOf(arg_steps));
            }
            else
            {
                outPutScript("scrollToBeginning2 " + String.valueOf(arg_maxSwipes) + " " + String.valueOf(arg_steps));
            }
            isScrollOK=scroll.scrollToBeginning(arg_maxSwipes,arg_steps);
        }
        catch(UiObjectNotFoundException e)
        {
            isScrollOK=false;
            if(isXXX)
            {
                outPutScript("scrollToBeginning2_notExit " + String.valueOf(arg_maxSwipes) + " " + String.valueOf(arg_steps) + "     princekin_notExit" + isScrollOK + "    NotFound");
            }
            else
            {
                outPutScript("scrollToBeginning2 " + String.valueOf(arg_maxSwipes) + " " + String.valueOf(arg_steps) + "     princekin" + isScrollOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("scrollToBeginning2_notExit " + String.valueOf(arg_maxSwipes) + " " + String.valueOf(arg_steps) + "     princekin_notExit" + isScrollOK);
        }
        else
        {
            outPutScript("scrollToBeginning2 " + String.valueOf(arg_maxSwipes) + " " + String.valueOf(arg_steps) + "     princekin" + isScrollOK);
        }
        //return isScrollOK;
    }
    //********************************** scrollToBeginning **********************************//      

    
    //******************************************************************************************//
    private void startApp(String arg_package)
    {
    	  String sec="";
        isStartOK=false;
        BufferedReader br=null;
        Process p=null;
        try
        {
            p=Runtime.getRuntime().exec("am start -W -n " + arg_package);
            InputStream stdin=p.getInputStream();
            InputStreamReader isr=new InputStreamReader(stdin,"gb2312");
            br=new BufferedReader(isr);
            String line=br.readLine();
            while(line!=null)
            {
                line=br.readLine();
                outPutScript(line);
                if(line!=null)
                {
                    if(line.startsWith("ThisTime:"))
                    {
                        String[] splitResult1=line.split(":");
                        sec=splitResult1[1].trim();
                    }
                }
            }
            p.waitFor();
        }
        catch(IOException e)
        {
        }
        catch(InterruptedException e)
        {
        }
        finally
        {
            try
            {
                p.getErrorStream().close();
                p.getInputStream().close();
                p.getOutputStream().close();
                br.close();
            }
            catch(IOException e)
            {
            }
        }
        String[] splitResult=arg_package.split("/");
        
        
        if(device.getCurrentPackageName().contains(splitResult[0]))
        {
        	  
            isStartOK=true;
        }
        else
        {
            isStartOK=false;
        }  
        outPutScript("startAppTime=" + sec);
        outPutScript("startApp=" + arg_package + "     princekin" + isStartOK);
    }
    
    
    
    private void install(String arg_apkName)
    {
        isInstallOK=false;
        BufferedReader br=null;
        Process p=null;
        try
        {
            p=Runtime.getRuntime().exec("pm install -r /sdcard/" + arg_apkName);
            InputStream stdin=p.getInputStream();
            InputStreamReader isr=new InputStreamReader(stdin,"gbk");
            br=new BufferedReader(isr);
            String line=br.readLine();
            if(line==null)
            {
                isInstallOK=false;
                outPutScript("install=" + arg_apkName + "     princekin" + isInstallOK);
                return;
            }
            while(line!=null)
            {
                if(line.contains("Success"))
                {
                    isInstallOK=true;
                    outPutScript("install=" + arg_apkName + "     princekin" + isInstallOK);
                    return;
                }
                if(line.contains("Failure"))
                {
                    isInstallOK=false;
                    outPutScript("install=" + arg_apkName + "     princekin" + isInstallOK);
                    return;
                }
                line=br.readLine();
                
            }
            p.waitFor();
        }
        catch(IOException e)
        {
        }
        catch(InterruptedException e)
        {
        }
        finally
        {
            try
            {
                p.getErrorStream().close();
                p.getInputStream().close();
                p.getOutputStream().close();
                br.close();
            }
            catch(IOException e)
            {
            }
        }
        
    }
    
    private void uninstall(String arg_packageName)
    {
        isUninstallOK=false;
        BufferedReader br=null;
        Process p=null;
        try
        {
            p=Runtime.getRuntime().exec("pm uninstall " + arg_packageName);
            InputStream stdin=p.getInputStream();
            InputStreamReader isr=new InputStreamReader(stdin,"gbk");
            br=new BufferedReader(isr);
            String line=br.readLine();
            if(line==null)
            {
                isUninstallOK=false;
                outPutScript("uninstall=" + arg_packageName + "     princekin" + isUninstallOK);
                return;
            }
            while(line!=null)
            {
                if(line.contains("Success"))
                {
                    isUninstallOK=true;
                    outPutScript("uninstall=" + arg_packageName + "     princekin" + isUninstallOK);
                    return;
                }
                if(line.contains("Failure"))
                {
                    isUninstallOK=false;
                    outPutScript("uninstall=" + arg_packageName + "     princekin" + isUninstallOK);
                    return;
                }
                line=br.readLine();
            }
            p.waitFor();
        }
        catch(IOException e)
        {
        }
        catch(InterruptedException e)
        {
        }
        finally
        {
            try
            {
                p.getErrorStream().close();
                p.getInputStream().close();
                p.getOutputStream().close();
                br.close();
            }
            catch(IOException e)
            {
            }
        }
        
    }
    
    
    
    private int runcmd(String arg_cmdline)
    {
        int x=0;
        BufferedReader br=null;
        Process p=null;
        try
        {
            p=Runtime.getRuntime().exec(arg_cmdline);
            InputStream stdin=p.getInputStream();
            InputStreamReader isr=new InputStreamReader(stdin,"gbk");
            br=new BufferedReader(isr);
            String line=br.readLine();
            while(line!=null)
            {
                if(line.contains("runok"))
                {
                    x=1;
                }
                else if(line.contains("runerror"))
                {
                    x=2;
                }
                else
                {
                    x=0;
                }
                
                line=br.readLine();
            }
            p.waitFor();
        }
        catch(IOException e)
        {
        }
        catch(InterruptedException e)
        {
        }
        finally
        {
            try
            {
                p.getErrorStream().close();
                p.getInputStream().close();
                p.getOutputStream().close();
                br.close();
            }
            catch(IOException e)
            {
            }
        }
        return x;
    }
    
    private void setHorizontal()
    {
        scroll.setAsHorizontalList();
    }
    private void setVertical()
    {
        scroll.setAsVerticalList();
    }
    private void setScrollInstance(UiSelector arg_selector)
    {
        scroll=new UiScrollable(arg_selector);
    }
    
    
    private UiObject getObj(UiSelector arg_selector)
    {
        UiObject mObj=new UiObject(arg_selector);
        return mObj;
    }
    public String getTimeYMD()
    {
        Date now;
        now=new Date();
        String mTime=fmt1.format(now);
        return mTime;
    }
    public String getTimeHMS()
    {
        Date now;
        now=new Date();
        String mTime=fmt2.format(now);
        return mTime;
    }
    
    private int getRandom(int range)
    {
    	  Random RandomInstance=new Random();
    	  return RandomInstance.nextInt(range);
    }
    
    
    private void inputText_notExit(String arg_Text)
    {
        isXXX=true;
        inputText(arg_Text);
    }
    private void inputText(String arg_Text)
    {
        try
        {
            if(isXXX)
            {
                outPutScript("inputText_notExit " + arg_Text);
            }
            else
            {
                outPutScript("inputText " + arg_Text);
            }
            isOK=obj.setText(arg_Text);
        }
        catch(UiObjectNotFoundException e)
        {
        	  isOK=false;
            if(isXXX)
            {
                outPutScript("inputText_notExit " + arg_Text + "     princekin_notExit" + isOK + "    NotFound");
            }
            else
            {
                outPutScript("inputText " + arg_Text + "     princekin" + isOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("inputText_notExit " + arg_Text + "     princekin_notExit" + isOK);
        }
        else
        {
            outPutScript("inputText " + arg_Text + "     princekin" + isOK);
        }
        //return isOK;
    }
    
    private void clearText_notExit()
    {
        isXXX=true;
        clearText();
    }
    private void clearText()
    {
        try
        {
            if(isXXX)
            {
                outPutScript("clearText_notExit");
            }
            else
            {
                outPutScript("clearText");
            }
            obj.clearTextField();
            isOK=true;
        }
        catch(UiObjectNotFoundException e)
        {
        	  isOK=false;
            if(isXXX)
            {
                outPutScript("clearText_notExit" + "     princekin_notExit" + isOK + "    NotFound");
            }
            else
            {
                outPutScript("clearText" + "     princekin" + isOK + "    NotFound");
            }
            isXXX=false;
            return;
        }
        if(isXXX)
        {
            isXXX=false;
            outPutScript("clearText_notExit" + "     princekin_notExit" + isOK);
        }
        else
        {
            outPutScript("clearText" + "     princekin" + isOK);
        }
        //return isOK;
    }
    
    private void take_Screen(String baseName)
    {
        String ymd=getTimeYMD();
        String hms=getTimeHMS();
        device.takeScreenshot(new File("/mnt/sdcard/temppng/" + baseName + "_" + ymd + "-" + hms + ".png"));
        sleep(3);
        System.out.println("princekinTakeScreen");
    }
    
    private void take_ScreenError(String baseName)
    {
        String ymd=getTimeYMD();
        String hms=getTimeHMS();
        device.takeScreenshot(new File("/mnt/sdcard/temppng/" + baseName + "_" + ymd + "-" + hms + "_error.png"));
        sleep(3);
        System.out.println("princekinTakeScreen");
    }
    
    
    private void sleep(int sec)
    {
        try
        {
            System.out.println("sleep " + String.valueOf(sec));
            Thread.sleep(sec*1000);
        }
        catch(InterruptedException e)
        {
        }
    }
    
    private void outPutScript(String arg_line)
    {
        System.out.println(arg_line);
    }
    //******************************************************************************************//
    private void monitor()
    {
        try
        {
            p=Runtime.getRuntime().exec("am monitor " + appnameStr);
        }
        catch(IOException e)
        {
        }
        InputStream stdin=null;
        stdin=p.getInputStream();
        Thread tIn=new Thread(new Receive(stdin));
        tIn.start();
    }
    
    class Receive implements Runnable
    {
        private InputStream ins;
        public Receive(InputStream ins)
        {
            this.ins=ins;
        }
        public void run()
        {
            try
            {
                InputStreamReader isr=new InputStreamReader(ins,"utf-8");
                BufferedReader br=new BufferedReader(isr);
                getRe(br);
            }
            catch(Exception e)
            {
            }
        }//run
    }
    
    private void getRe(BufferedReader br)
    {
        try
        {
            String line=br.readLine();
            while(line!=null)
            {
                if(line.toUpperCase().contains(crashStr1) || line.toUpperCase().contains(crashStr2) || line.toUpperCase().contains(nullStr1))
                {
                    outPutScript(line);
                    take_ScreenError(fileName);
                    outPutScript("monitorError");
                    return;
                }
                line=br.readLine();
            }//while(line!=null)
            //p.waitFor();
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
    }
    
    private void fastBack()
    {
        System.out.println("fastBack");
        //System.out.println("BACK" + "=" + isPressOK);
    }
    
    private void pushApk(String arg_apk)
    {
        System.out.println("pushApk=" + arg_apk);
    }
    
    private void checkStatPoint(String arg_str)
    {
    	  try
    	  {
    	      if(checkStatStr.startsWith("no"))
    	      {
    	      	  System.out.println("该模块不支持统计点检测");
    	          return;
    	      }
    	  }
    	  catch(Exception e)
    	  {
    	  }

        String[] array=arg_str.split(" ");
        String str="";
        int len=array.length;
        for(int i=0;i<len;i++)
        {
            if(!array[i].trim().equals(""))
            {
                if(i==len-1)
                {
                    str=str+array[i];
                }
                else
                {
                    str=str+array[i] + "=";
                }
            }
        }
        
        try
        {
            Thread.sleep(2000);
        }
        catch (InterruptedException e)
        {
        }
        System.out.println(str);
        try
        {
            Thread.sleep(2000);
        }
        catch (InterruptedException e)
        {
        }
    }
    
    private final String crashStr1="ERROR: PROCESS CRASHED";
    private final String crashStr2="CRASH";
    private final String nullStr1="JAVA.LANG.NULLPOINTEREXCEPTION";
    private final String nullStr2="java.lang.NullPointerException";
    
    
    
    private boolean isXXX=false;
    private boolean isOK=false;
    private boolean isPressOK=false;
    private boolean isClickOK=false;
    private boolean isCheckOK=false;
    private boolean isScrollOK=false;
    private boolean isStartOK=false;
    private boolean isInstallOK=false;
    private boolean isUninstallOK=false;
    private boolean isOK1=false;
    private boolean isOK2=false;
    
    
    private final String strCancel="取消";
    private final String strConfirm="确定";
    private final String strHot="热点";
    private String checkStatStr=null;
    private final String hotResourceId="com.sohu.sohuvideo:id/tv_hot";
    private final String appname="com.sohu.sohuvideo";
    private String appnameStr="";
    
    
    private Process p=null;
    private UiObject obj;
    private UiDevice device;
    private UiSelector selector;
    private UiScrollable scroll;
    private Rect rectxy;
    private Matcher match;
    private Pattern pattern=Pattern.compile("(\\d+)");
    private final SimpleDateFormat fmt1=new SimpleDateFormat("yyyyMMdd");
    private final SimpleDateFormat fmt2=new SimpleDateFormat("HH-mm-ss");

}

