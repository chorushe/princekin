package com.sohu.princekin.autotravel;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.text.DateFormat;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import android.os.Bundle;
import android.os.Environment;
import android.provider.ContactsContract.DeletedContacts;

import com.android.uiautomator.core.UiDevice;
import com.android.uiautomator.core.UiObject;
import com.android.uiautomator.core.UiObjectNotFoundException;
import com.android.uiautomator.core.UiSelector;
import com.android.uiautomator.testrunner.UiAutomatorTestCase;
import com.sohu.princekin.autotravel.UiAutomatorHelper;



/**
 * 以activity作为key，resourceid和text联合作为控件的标识
 * 去掉包含back字段的控件 20170110
 * @author changhe
 *
 */

class FileListenerThread implements Runnable{
	
	private String path;
	static boolean flag=false;
	@Override
	public void run() {
		// TODO Auto-generated method stub
		File file = new File(path);
		while(!flag)
		{
			if(file.exists())
			{
				flag=true;
				
				TravelClass.isContinue=false;
			}
			try {
				Thread.sleep(2000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	public FileListenerThread(String path){
        this.path= path;
    }
	
}

public class TravelClass extends UiAutomatorTestCase {

	UiDevice device;
	private Document doc;
	private XPath xpath;
	private Map<String,List<String>> activityMapId=new HashMap<String,List<String>>();
	private Map<String,List<widgetInfo>> activityMap=new HashMap<String,List<widgetInfo>>();
	private List<String> activityList=new ArrayList<String>();
	private String testAppPackage="";
	private final SimpleDateFormat fmt1=new SimpleDateFormat("yyyyMMdd");
	private final SimpleDateFormat fmt2=new SimpleDateFormat("HH-mm-ss");
	private NodeList nodeList=null;
	String currentActivity="";
	int ClickedNum=0;
	int ExceptionNum=0;
	String[] blackId;
	int time=3000;
	boolean dumpErrorFlag=false;
	// private NodeList allNodeList=null;
	public static boolean isContinue=true;
    
	/**
	 * @param args
	 * @throws InterruptedException 
	 */
	public static void main(String[] args) throws InterruptedException {
		// TODO Auto-generated method stub
        
		String jarName="TravelTest";
		String testClass="com.sohu.princekin.autotravel.TravelClass";
		String testName="testTravel";
		String androidId="2";
		new UiAutomatorHelper(jarName, testClass, testName, androidId); 
       
	}
	
	
	/**
	 * 实验点击resourceid
	 * @throws IOException 
	 * @throws UiObjectNotFoundException 
	 */
	public void testClickWait() throws IOException 
	{
		init();
		try
		{
			UiObject obj=new UiObject(new UiSelector().text("重新再试"));
			System.out.println("第一次点击     "+obj.getText()+obj.getClassName());
			boolean flag = obj.clickAndWaitForNewWindow(3000);
			System.out.println(flag);
		}
		catch(UiObjectNotFoundException e)
		{
			System.out.println(e.getMessage()+"找不到控件");
		}
		catch(Exception e)
		{
			System.out.println(e.getMessage()+"异常");
		}
		
		try
		{
			UiObject obj=new UiObject(new UiSelector().text("我"));
			System.out.println("第二次点击     "+obj.getText()+obj.getClassName());
			boolean flag = obj.click();
			System.out.println(flag);
			sleep(5000);
		}
		catch(UiObjectNotFoundException e)
		{
			System.out.println(e.getMessage()+"找不到控件");
		}
		catch(Exception e)
		{
			System.out.println(e.getMessage()+"异常");
		}
	}
	
	public void testPara()
	{
		/*Bundle bundle=getParams();//获取键值对  
        String userName=bundle.getString("userName");  
        String password=bundle.getString("password");  
        long runTime=Long.parseLong(bundle.getString("runTime"));  
        System.out.println(runTime+"======="+password+"========"+userName); */
		this.device = getUiDevice();
		getCurrentActivity2();

	}

	/**
	 * 初始化
	 * @throws IOException 
	 */
	private void init() throws IOException
	{
		this.device = getUiDevice();
		//建立
		File file = new File("/data/local/tmp/local/png");
		if (file.exists())
		{
			deleteFile(file);
		}
		file.mkdirs();
		file = new File("/data/local/tmp/local/errorpng");
		if (file.exists())
		{
			deleteFile(file);
		}
		file.mkdirs();
		sleep(2000);
		//System.out.println("开始自动遍历... "+getTimeYMD()+"-"+getTimeHMS());
		System.out.println("开始自动遍历... ");
		/*//获取手机型号
		String deviceModel=getAdbResult("getprop ro.product.model");
		System.out.println(deviceModel);
		//获取手机SN号
		String deviceSn=getAdbResult("getprop ro.serialno");
		System.out.println(deviceSn);
		//获取手机系统版本
		String deviceVersion=getAdbResult("getprop ro.build.version.release");
		System.out.println(deviceVersion);*/
		
		String path="/data/local/tmp/local/stop.txt";
		Thread listener=new Thread(new FileListenerThread(path));
		listener.start();
	}

	/**
	 * 查看dump.xml文件中resourceid和text都是空的情况
	 * @throws Exception 
	 */
	public void testResourceidText() throws Exception
	{
		init();
		dumpXML();
		System.out.println("dump done!");
		xmlInit();
		if(xpath==null)
			return;
		nodeList = (NodeList) xpath.evaluate("//node[@clickable='true']", doc,XPathConstants.NODESET);
		int resoucrid=0;
		int text=0;
		int both=0;
		for(int i=1;i<nodeList.getLength();i++)
		{
			if(nodeList.item(i).getAttributes().getNamedItem("resource-id").getNodeValue().equals(""))
				resoucrid++;
			if(nodeList.item(i).getAttributes().getNamedItem("text").getNodeValue().equals(""))
				text++;
			if(nodeList.item(i).getAttributes().getNamedItem("resource-id").getNodeValue().equals("")&&nodeList.item(i).getAttributes().getNamedItem("text").getNodeValue().equals(""))
				both++;
		}
		System.out.println(resoucrid);
		System.out.println(text);
		System.out.println(both);
	}

	/**
	 * 实验点击resourceid
	 * @throws IOException 
	 * @throws UiObjectNotFoundException 
	 */
	public void tesrResourceId() throws IOException 
	{
		init();
		try
		{
			UiObject obj=new UiObject(new UiSelector().resourceId("com.sohu.sohuvideo:id/id_search_filter_tv"));
			System.out.println("第一次点击     "+obj.getText()+obj.getClassName());
			obj.click();
		}
		catch(UiObjectNotFoundException e)
		{
			System.out.println(e.getMessage()+"找不到控件");
		}
		catch(Exception e)
		{
			System.out.println(e.getMessage()+"异常");
		}
	}

	/**
	 * 遍历函数
	 * @throws Exception
	 */
	public void testTravel() throws Exception
	{
		SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");//设置日期格式
		String startTime=df.format(new Date());
		//获取传参
		Bundle bundle=getParams();
		String blackResourceId=bundle.getString("blackName-resouceId");
		if(blackResourceId!="NULL")
			blackId=blackResourceId.split("=");
		String timeStr=bundle.getString("time");
		//System.out.println(timeStr);
		if(timeStr!="NULL")
			time=Integer.parseInt( timeStr );
		/*System.out.println(time);*/
		try
		{
			init();
			dumpXML();
			if(dumpErrorFlag)
				return;
			xmlInit();
			if(xpath==null)
				return;
			//直接获取所有节点，不筛选clickable=true,因为下面要计算之前出现过的index，需要知道该节点在列表里的位置
			//nodeList = (NodeList) xpath.evaluate("//node[@clickable='true']", doc,XPathConstants.NODESET);
			nodeList = (NodeList) xpath.evaluate("//node[@clickable='true']", doc,XPathConstants.NODESET);
			
			currentActivity=getCurrentActivity();
			testAppPackage = getCurrentAppPackageFromActivity(currentActivity);
			//System.out.println(testAppPackage);
			if(!activityList.contains(currentActivity))
				activityList.add(currentActivity);

			addActivityList();
			UiObject obj=null;
			try
			{
				
				if(!activityMap.get(currentActivity).get(0).resourceid.equals(""))
				{
					obj=new UiObject(new UiSelector().resourceId(activityMap.get(currentActivity).get(0).resourceid));
				}
				else
				{
					obj=new UiObject(new UiSelector().text(activityMap.get(currentActivity).get(0).text));
				}
				System.out.println("startClickFlag");
				String str="点击控件     "+obj.getText()+"   "+obj.getClassName();//要先获取text和className，否则点击之后如果页面跳转了，在新页面找不到该控件，再获取text会找不到控件
				obj.click();
				sleep(time);
				System.out.println(str);
				takeScreen();
				System.out.println("endClickFlag");
				
				
				ClickedNum++;

				widgetInfo tmpwidget=activityMap.get(currentActivity).get(0);
				tmpwidget.isClicked=true;
				activityMap.get(currentActivity).set(0, tmpwidget);
			}
			catch(UiObjectNotFoundException e)
			{
				String pngName=takeErrorScreen();
				System.out.println("点击异常      找不到控件       "+e.getMessage());
				ExceptionNum++;
			}
			catch(Exception e)
			{
				String pngName=takeErrorScreen();
				if(obj!=null)
					System.out.println("点击异常     "+obj.getText()+"   "+obj.getClassName()+"   "+e.getMessage());
				else
					System.out.println("点击异常     "+e.getMessage());
				ExceptionNum++;
			}
			while(isContinue)//如果activity不包含被测应用的包名，则退出循环
			{
				dumpXML();
				if(dumpErrorFlag)
					continue;
				xmlInit();
				if(xpath==null)
					continue;
				
				nodeList = (NodeList) xpath.evaluate("//node[@clickable='true']", doc,XPathConstants.NODESET);
				currentActivity=getCurrentActivity();
				if(!currentActivity.contains(testAppPackage))
				{
					System.out.print("=$应用已换，关闭当前应用...");
					String currentPackage=getCurrentAppPackageFromActivity(currentActivity);
					//System.out.println("1"+currentPackage);
					killPackage(currentPackage);
					sleep(3000);
					//System.out.println("dengdai 3miao");
					//continue;
					currentActivity=getCurrentActivity();
					String currentPackage2=getCurrentAppPackageFromActivity(currentActivity);
					//System.out.println("2"+currentPackage);
					sleep(2000);
					if(currentPackage.equals(currentPackage2))
					{
						System.out.println("无法返回至原应用，此次遍历结束！\r\n");
						break;
					}
					else
					{
						continue;
					}/**/
					/*device.pressBack();
					currentActivity=getCurrentActivity();
					if(!currentActivity.contains(testAppPackage))
					{
						System.out.println("并未返回至原应用，连续点击两次返回按键...");
						device.pressBack();
						device.pressBack();
						currentActivity=getCurrentActivity();
						if(!currentActivity.contains(testAppPackage))
						{
							System.out.println("无法返回至原应用，此次遍历结束！");
							break;
						}
					}*/
				}

				//此时分为三种情况，1、activity变化；2、activity没变但页面有跳转，有新控件产生；3、页面和activity都没变化
				if(!activityList.contains(currentActivity))//新的activity
				{
					//System.out.println("Activity发生变化"); 
					activityList.add(currentActivity);

					addActivityList();

					UiObject obj1=null;
					try
					{
						
						if(!activityMap.get(currentActivity).get(0).resourceid.equals(""))
						{
							obj1=new UiObject(new UiSelector().resourceId(activityMap.get(currentActivity).get(0).resourceid));
						}
						else
						{
							obj1=new UiObject(new UiSelector().text(activityMap.get(currentActivity).get(0).text));
						}
						System.out.println("startClickFlag");
						String str="点击控件      "+obj1.getText()+"   "+obj1.getClassName();
						
						obj1.click();
						sleep(time);
						System.out.println(str);
						takeScreen();
						System.out.println("endClickFlag");
						
						
						ClickedNum++;
						
						if(activityMap.get(currentActivity).size()>0)
						{
							widgetInfo tmpwidget1=activityMap.get(currentActivity).get(0);
							tmpwidget1.isClicked=true;
							activityMap.get(currentActivity).set(0, tmpwidget1);
						}
					}
					catch(UiObjectNotFoundException e)
					{
						String pngName=takeErrorScreen();
						System.out.println("点击异常      找不到控件       "+e.getMessage());
						ExceptionNum++;
					}
					catch(Exception e)
					{
						String pngName=takeErrorScreen();
						if(obj1!=null)
							System.out.println("点击异常     "+obj1.getText()+"   "+obj1.getClassName()+"   "+e.getMessage());
						else
							System.out.println("点击异常     "+e.getMessage());
						ExceptionNum++;
					}

				}
				else//activity没变
				{
					List<String> widgetsInActivity2=new ArrayList<String>();
					List<widgetInfo> widgetsInActivityList2=new ArrayList<widgetInfo>();
					for(int i=0;i<nodeList.getLength();i++)
					{
						String resourceid=nodeList.item(i).getAttributes().getNamedItem("resource-id").getNodeValue();
						String text=nodeList.item(i).getAttributes().getNamedItem("text").getNodeValue();
						//判断id是否是黑名单中的控件
						boolean BlackFlag=false;
						for(int k=0;k<blackId.length;k++)
						{
							if(resourceid.contains(blackId[k]))
							{
								BlackFlag=true;
								break;
							}
						}
						if(BlackFlag)
							continue;
						if(!(resourceid.trim().equals("")&&text.equals("")))
						{
							if(resourceid.trim().equals(""))
								widgetsInActivity2.add(text);
							else
								widgetsInActivity2.add(resourceid);
							widgetInfo tempWidget=new widgetInfo();
							tempWidget.resourceid=resourceid;
							tempWidget.isClicked=false;
							tempWidget.text=text;

							widgetsInActivityList2.add(tempWidget);
						}
					}
					int size=0;
					for(int k=0;k<widgetsInActivity2.size();k++)
					{
						if(!activityMapId.get(currentActivity).contains(widgetsInActivity2.get(k)))
						{
							if(size==0)//该页面内的第一个控件需要进行点击操作
							{
								UiObject obj2=null;
								try
								{
									//System.out.println("Activity没有变化，但有新控件产生");

									
									if(!widgetsInActivityList2.get(k).resourceid.equals(""))
										obj2=new UiObject(new UiSelector().resourceId(widgetsInActivityList2.get(k).resourceid));
									else
										obj2=new UiObject(new UiSelector().text(widgetsInActivityList2.get(k).text));
									System.out.println("startClickFlag");
									String str="点击控件      "+obj2.getText()+"   "+obj2.getClassName();
									
									obj2.click();
									sleep(time);
									System.out.println(str);
									takeScreen();
									System.out.println("endClickFlag");
									
									ClickedNum++;

									activityMapId.get(currentActivity).add(widgetsInActivity2.get(k));
									widgetInfo tempwidget=widgetsInActivityList2.get(k);
									tempwidget.isClicked=true;
									activityMap.get(currentActivity).add(tempwidget);
								}
								catch(UiObjectNotFoundException e)
								{
									String pngName=takeErrorScreen();
									System.out.println("点击异常      找不到控件       "+e.getMessage());
									ExceptionNum++;
								}
								catch(Exception e)
								{
									String pngName=takeErrorScreen();
									if(obj2!=null)
										System.out.println("点击异常       "+obj2.getText()+"   "+obj2.getClassName()+"   "+e.getMessage());
									else
										System.out.println("点击异常       "+e.getMessage());
									ExceptionNum++;
								}
								size++;
							}
							else
							{
								activityMapId.get(currentActivity).add(widgetsInActivity2.get(k));
								activityMap.get(currentActivity).add(widgetsInActivityList2.get(k));
								size++;
							}
						}
					}
					if(size==0)//activity没有变化，也没有新控件产生
					{
						//System.out.println("Activity没有变化，没有新控件产生");
						boolean isCurrentAllClicked=true;//
						//在Map中找widgetsInActivity2序列中没有点击过的resourceid，点击
						for(int m=0;m<widgetsInActivity2.size();m++)
						{
							int index=activityMapId.get(currentActivity).indexOf(widgetsInActivity2.get(m));
							if(activityMap.get(currentActivity).get(index).isClicked==true)//被点击过，跳过
							{
								continue;
							}
							else//找到一个未被点击的，点击，并跳出
							{
								//	if(m!=0)
								//	{
								//		System.out.println(m+"个控件被点过，跳下一个未点击控件...");
								//	}
								UiObject obj3 = null;
								try
								{
									
									if(!widgetsInActivityList2.get(m).resourceid.equals(""))
										obj3=new UiObject(new UiSelector().resourceId(widgetsInActivityList2.get(m).resourceid));
									else
										obj3=new UiObject(new UiSelector().text(widgetsInActivityList2.get(m).text));
									System.out.println("startClickFlag");
									String str="点击控件     "+obj3.getText()+"   "+obj3.getClassName();
									
									obj3.click();
									sleep(time);
									System.out.println(str);
									takeScreen();
									System.out.println("endClickFlag");
									
									ClickedNum++;

									widgetInfo tempwidget=widgetsInActivityList2.get(m);
									tempwidget.isClicked=true;
									activityMap.get(currentActivity).set(index, tempwidget);
									isCurrentAllClicked=false;
									break;
								}
								catch(UiObjectNotFoundException e)
								{
									String pngName=takeErrorScreen();
									System.out.println("点击异常      找不到控件       "+e.getMessage());
									ExceptionNum++;
								}
								catch(Exception e)
								{
									String pngName=takeErrorScreen();
									if(obj3!=null)
										System.out.println("点击异常       "+obj3.getText()+"   "+obj3.getClassName()+"   "+e.getMessage());
									else
										System.out.println("点击异常       "+e.getMessage());
									ExceptionNum++;
								}

							}
						}
						if(isCurrentAllClicked)//当前页面全部被点击过了，回退
						{
							device.pressBack();
							System.out.println("当前页面均已被点击过，点击返回按键");
						}
					}//if(size==0)
				}//activity没变化
			}//while
			
			if(FileListenerThread.flag)
			{
				sleep(2000);
				System.out.println("手动停止遍历...");
			}
		}
		catch(Exception e)
		{
			System.out.println("出现异常     "+e.getMessage());
		}
		finally
		{
			sleep(2000);
		//	System.out.println("统计数据-------------------");
			
			System.out.print("=$遍历开始时间：  "+startTime);
			String endTime=df.format(new Date());
			System.out.print("=$遍历结束时间：  "+endTime);// new Date()为获取当前系统时间

			Date d1=df.parse(endTime);
			Date d2=df.parse(startTime);
			long diff = d1.getTime() - d2.getTime();//这样得到的差值是微秒级别

			long days = diff / (1000 * 60 * 60 * 24);
			long hours = (diff-days*(1000 * 60 * 60 * 24))/(1000* 60 * 60);
			long minutes = (diff-days*(1000 * 60 * 60 * 24)-hours*(1000* 60 * 60))/(1000* 60);
			long seconds = (diff-days*(1000 * 60 * 60 * 24)-hours*(1000* 60 * 60)-minutes*(1000*60))/1000;
			System.out.print("=$遍历时长：  "+days+"天"+hours+"小时"+minutes+"分"+seconds+"秒=$-----------------------------------------------------"); 
			
			

			if(activityMapId.size()>0)
			{
				int widgetsAll=0;
				String resultStr="";
				for (Map.Entry<String, List<String>> entry : activityMapId.entrySet()) 
				{  
					int widgetsClicked=0;
					String activityName=entry.getKey();
					List<String> widgetsList=entry.getValue();
					widgetsAll+=widgetsList.size();
					for(int i=0;i<widgetsList.size();i++)
			        {
			        	if(activityMap.get(activityName).get(i).isClicked==true)
			        		widgetsClicked++;
			        }
			        resultStr+="=$" +activityName+" : 检测到的控件数量： "+widgetsList.size()+", 点击到的控件数量： "+widgetsClicked+"\r\n\r\n";
				}  
				System.out.println("=$检测到的控件总数： "+widgetsAll+"  点击到的控件个数：  "+ClickedNum +"  点击异常控件个数：  "+ExceptionNum);
				DecimalFormat df0 = new DecimalFormat("###.00");
				System.out.println("=$遍历过的Activity数量："+activityMapId.size());
				System.out.println(resultStr);
				System.out.println("=$遍历比："+df0.format((double)ClickedNum/(double)widgetsAll*100)+"%\r\n");
			}

			
			//System.out.println("ALLSTOP!");
			sleep(2000);
		}
	}

	/**
	 * 添加activity的控件列表
	 */
	private void addActivityList()
	{
		List<String> widgetsInActivity=new ArrayList<String>();
		List<widgetInfo> widgetInfoList=new ArrayList<widgetInfo>();
		for(int i=0;i<nodeList.getLength();i++)
		{			
			String resourceid=nodeList.item(i).getAttributes().getNamedItem("resource-id").getNodeValue();
			String text=nodeList.item(i).getAttributes().getNamedItem("text").getNodeValue();
			//判断id是否是黑名单中的控件
			boolean BlackFlag=false;
			for(int k=0;k<blackId.length;k++)
			{
				if(resourceid.contains(blackId[k]))
				{
					BlackFlag=true;
					break;
				}
			}
			if(BlackFlag)
				continue;
			if(!(resourceid.trim().equals("")&&text.equals("")))//排除掉text和id都是空的情况
			{
				if(resourceid.trim().equals(""))
					widgetsInActivity.add(text);
				else
					widgetsInActivity.add(resourceid);
				widgetInfo tempWidget=new widgetInfo();
				tempWidget.resourceid=resourceid;
				tempWidget.isClicked=false;
				tempWidget.text=text;
				widgetInfoList.add(tempWidget);
			}
		}
		activityMapId.put(currentActivity, widgetsInActivity);
		activityMap.put(currentActivity, widgetInfoList);
	}

	/**
	 * 获取所谓的xpath,由每个节点的父节点的classname+bounds构成
	 */
	private String getXpath(Node node)
	{
		String xpathStr=node.getAttributes().getNamedItem("class").getNodeValue();
		Node parent=null;
		parent=node.getParentNode();
		while( parent.getNodeName().equals("node") )
		{
			xpathStr += parent.getAttributes().getNamedItem("class").getNodeValue();
			parent=parent.getParentNode();
		}
		xpathStr+=node.getAttributes().getNamedItem("bounds").getNodeValue();
		xpathStr=GenerateMD5.getMd5(xpathStr);
		//System.out.println("xpath: "+xpathStr);

		return xpathStr ;
	}

	/**
	 * 计算改控件的类的index
	 */
	private int getIndex(Node node,int num)
	{
		String className=node.getAttributes().getNamedItem("class").getNodeValue();
		int count=-1;
		for(int i=1;i<=num;i++)
		{
			if(nodeList.item(i).getAttributes().getNamedItem("class").getNodeValue().equals(className))
				count++;
		}
		//System.out.println(count);
		return count;
	}

	/**
	 *递归删除文件夹所有内容
	 *
	 */
	private void deleteFile(File file) 
	{
		if (file.isFile()) { // 判断是否是文件
			file.delete(); // delete()方法 你应该知道 是删除的意思;
		} else if (file.isDirectory()) { // 否则如果它是一个目录
			File files[] = file.listFiles(); // 声明目录下所有的文件 files[];
			for (int i = 0; i < files.length; i++) { // 遍历目录下所有的文件
				this.deleteFile(files[i]); // 把每个文件 用这个方法进行迭代
			}
		}
		file.delete();
	}

	/**
	 * 获取xml文件内容之前的初始化
	 */
	private void xmlInit() throws Exception {
		// 创建Document对象
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		dbf.setValidating(false);
		DocumentBuilder db = dbf.newDocumentBuilder();
		File file=new File("/data/local/tmp/local/tmp/dump.xml");
		if(file.exists())
		{
			doc = db.parse(new FileInputStream(file));

			// 创建XPath对象
			XPathFactory factory = XPathFactory.newInstance();
			xpath = factory.newXPath();
		}
		else
		{
			xpath=null;
			//System.out.println("dump.xml文件不存在，导出失败！");
		}
	}

	/**
	 * 导出xml文件
	 */
	private void dumpXML()
	{
		File file = new File(new File(Environment.getDataDirectory(), "local/tmp"), "dump.xml");
		file.mkdirs();
		if (file.exists()) 
		{
			file.delete();
		}
		try
		{
			this.device.dumpWindowHierarchy("dump.xml");
			dumpErrorFlag=false;
		}
		catch(Exception e)
		{
			dumpErrorFlag=true;
			System.out.println("dump异常     "+e.getMessage());
		}
	}

	/**
	 * 获取当前运行的activity名称
	 */
	private String getCurrentActivity()
	{
		String adbLine="dumpsys activity";
		String activityName="";
		//System.out.println("获取activity");
		BufferedReader br=null;
		Process p=null;
		try
		{
			try
			{
				//"dumpsys activity | grep mFocusedActivity"
				p=Runtime.getRuntime().exec(adbLine);
				InputStream stdin=p.getInputStream();
				InputStreamReader isr=new InputStreamReader(stdin,"gbk");
				br=new BufferedReader(isr);
				String line=br.readLine();
				while(line!=null)
				{
					if(line.contains("mResumedActivity"))
					{
						activityName=line;
					//	System.out.println("111"+activityName);
					}
					line=br.readLine();

				}

				InputStream stderr=p.getErrorStream();
				InputStreamReader errisr=new InputStreamReader(stderr,"gbk");
				br=new BufferedReader(errisr);
				String lineerr=br.readLine();
				while(lineerr!=null)
				{
					System.out.println(lineerr);
					lineerr=br.readLine();
				}

				p.waitFor();
			}
			finally
			{

				p.getErrorStream().close();
				p.getInputStream().close();
				p.getOutputStream().close();

			}
		}
		catch(IOException e)
		{
			System.out.println("getactivity: "+e.getMessage());
		}
		catch(InterruptedException e)
		{
			System.out.println("getactivity2: "+e.getMessage());
		}

		if(activityName!="")
		{
			String[] list = activityName.split(" ");
			String activity = list[list.length-2];
			System.out.println("获取Activity   "+activity);
			return activity;
		}
		return "";
	}
	
	private void getCurrentActivity2()
	{
		 String line = null;  
		 StringBuilder sb = new StringBuilder();  
		    Runtime runtime = Runtime.getRuntime();  
		    try {  
		    Process process = runtime.exec("dumpsys activity");  
		    BufferedReader  bufferedReader = new BufferedReader  
		            (new InputStreamReader(process.getInputStream()));  
		      
		      
		        while ((line = bufferedReader.readLine()) != null) {  
		            sb.append(line + "\n");  
		            if (line.contains("mResumedActivity")) {  
		                System.out.println(line);  
		            }  
		        }  
		    } catch (IOException e) {  
		        // TODO 自动生成的 catch 块  
		        e.printStackTrace();  
		    }  
	}
	

	/**
	 * 从当前activity名称中提取当前应用包名
	 */
	private String getCurrentAppPackageFromActivity(String currentActivity) 
	{
		//com.sohu.qianfan/.ui.activity.HomePageActivity
		if(currentActivity.length()<=0)
		{
			System.out.println("activity为空");
			return "";
		}
		String currentPackage="";
		int index=currentActivity.indexOf("/");
		currentPackage=currentActivity.substring(0,index);
		return currentPackage;
	}

	/**
	 * 运行并获取获取adb结果
	 */
	private String getAdbResult( String adbLine) throws IOException
	{
		BufferedReader br=null;
		Process p=null;
		String line="";
		try
		{
			p=Runtime.getRuntime().exec(adbLine);
			InputStream stdin=p.getInputStream();
			InputStreamReader isr=new InputStreamReader(stdin,"gbk");
			br=new BufferedReader(isr);
			line=br.readLine();
			while(line!=null)
			{
				//System.out.println(line);
				line+=br.readLine();
			}

			InputStream stderr=p.getErrorStream();
			InputStreamReader errisr=new InputStreamReader(stderr,"gbk");
			br=new BufferedReader(errisr);
			String lineerr=br.readLine();
			while(lineerr!=null)
			{
				//System.out.println(lineerr);
				lineerr+=br.readLine();
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
			}
			catch(IOException e)
			{}
		}

		return line;
	}

	/**
	 * 杀死正在运行的应用
	 * @param packageName
	 * @throws IOException
	 */
	public void killPackage(String packageName) throws IOException
	{
		getAdbResult("am force-stop "+packageName);
		sleep(4000);
	}
	
	/**
	 * 截图
	 */
	private void takeScreen() throws IOException
	{
		String ymd=getTimeYMD();
		String hms=getTimeHMS();

		device.takeScreenshot(new File( "/data/local/tmp/local/png/"+ymd+"_"+hms+".png"));
		System.out.println("takeScreen  "+ymd+"_"+hms+".png");
	}
	
	/**
	 * 截图，返回图片名称
	 */
	private String takeErrorScreen() throws IOException
	{
		String ymd=getTimeYMD();
		String hms=getTimeHMS();

		device.takeScreenshot(new File( "/data/local/tmp/local/errorpng/"+ymd+"_"+hms+".png"));
		System.out.println("takeScreen  "+ymd+"_"+hms+".png");
		return ymd+"_"+hms+".png";
	}

	/**
	 * 获取时间年月日
	 */
	public String getTimeYMD()
	{
		Date now;
		now=new Date();
		String mTime=fmt1.format(now);
		return mTime;
	}

	/**
	 * 获取时间时分秒
	 */
	public String getTimeHMS()
	{
		Date now;
		now=new Date();
		String mTime=fmt2.format(now);
		return mTime;
	}
	

}
