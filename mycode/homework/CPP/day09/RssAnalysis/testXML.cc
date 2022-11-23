#include<iostream>
#include<fstream>
#include<regex>
#include"tinyxml2.h"
#include"Rss.h"
using namespace tinyxml2;
using std::cout;
using std::endl;
using std::regex;
/*CREAT XMLDocument*/
void test0(){
    const char *xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>   \
                        <note>                      \
                            <to>beijing</to>             \
                            <from>shenzhen</from>           \
                            <heading>Reminder</heading> \
                            <body>Don't forget the meeting!</body> \
                        </note>";

    tinyxml2::XMLDocument doc;
    //解析xml字符串,存放在对象doc中
    doc.Parse(xml);
    std::cout << doc.ErrorID() << std::endl;

    // 1. 第一种刷新到本地
    FILE *fp = fopen("Test0.xml", "wb");//fopen--->fstream
    tinyxml2::XMLPrinter printer(fp);
    doc.Print(&printer); // 打印到文件，则.CStr()就返回空值了
    std::cout << "xml:" << printer.CStr() << std::endl;//ErrorID=0:SUCCESS
    fclose(fp);
    //

    // 2. 第二种刷新到本地
    doc.SaveFile("Test1.xml");
    //
}
/*OPEN XMLDocument*/
void test1(){
    /*ONE*/
    XMLDocument xmlDocument;
	if (xmlDocument.LoadFile("Test0.xml"))//成功返回0
	{   //errorID=1:XML_NO_ATTRIBUT
        //errorID=3:XML_ERROR_FILE_NOT_FOUND
        //errorID=8:XML_ERROR_PARSING_TEXT
		cout<<"fail:"<<xmlDocument.ErrorID()<<endl;
    }else{
        cout<<"success:"<<xmlDocument.ErrorID()<<endl;
    }

    /*TWO*/
    XMLElement* xmlRoot = xmlDocument.RootElement();
	if (xmlRoot == NULL)
	{
        cout<<"XmlRoot=NULL:"<<xmlDocument.ErrorID()<<endl;
	}

#if 0
	//获取子节点信息1
	XMLElement* xmlNode1Name = xmlRoot->FirstChildElement("name");
	const char* node1Name = xmlNode1Name->GetText();
    cout<<"node1Name"<<node1Name<<endl;
	const char* node1AttId = xmlNode1Name->Attribute("id");
    cout<<"node1AttId"<<node1AttId<<endl;
	//获取子节点信息2
	XMLElement* xmlNode2Stu = xmlRoot->FirstChildElement("Student");
	XMLElement* xmlNode2_name = xmlNode2Stu->FirstChildElement("name");
	XMLElement* xmlname2_classes = xmlNode2Stu->FirstChildElement("classes");
	const char* node2Name = xmlNode2_name->GetText();
    cout<<"node2Name"<<node2Name<<endl;

	const char* node2Classes = xmlname2_classes->GetText();
    cout<<"node2Classes"<<node2Classes<<endl;
#endif
}
void test2(){
    /*THREE*/
    XMLDocument xmlDocument;
    xmlDocument.LoadFile("coolshell.xml");
    XMLElement *category = xmlDocument.RootElement();
    XMLElement *channel = category->FirstChildElement("channel");//根节点第一个孩子channel
    //XMLElement *title = channel->FirstChildElement("title");//拿到channel的孩子结点title
    XMLElement *item = channel->FirstChildElement("item");//channel结点有很多个item子结点
    while(item)//遍历channel下的每个item结点
    {
        
        RssItem rss_item;
        RssReader RssR;
        regex reg("<[^>]*>"); 
 
   		//获取title结点指针，把内容保存到结构体相应的变量
        XMLElement *item_title = item->FirstChildElement("title");//title结点
        //printf("111\n");
        if(item_title)
            rss_item.title = item_title->GetText(); //保存结点的内容
		
        //获取link结点指针，把内容保存到结构体相应的变量
        XMLElement *item_link = item->FirstChildElement("link");
        if(item_link)
        rss_item.link= item_link->GetText();
        //printf("222\n");
        //rss_item.link=regex_replace(rss_item.link,reg,"");
        cout<<rss_item.link<<endl;
 
		//获取desc结点指针，数据用正则去除html标签后，把内容保存到结构体相应的变量
        XMLElement *item_description = item->FirstChildElement("description");
        rss_item.description= item_description->GetText();
        //printf("333\n");
 
        rss_item.description = regex_replace(rss_item.description,reg,"");//去除html标签
 
        //获取content结点指针，数据用正则去除html标签后，把内容保存到结构体相应的变量
        XMLElement *item_content = item->FirstChildElement("content:encoded");
        rss_item.content= item_content->GetText();
        cout<<rss_item.content<<endl;
        rss_item.content = regex_replace(rss_item.content,reg,"");//去除网页标签
        cout<<rss_item.content<<endl;
        
        //把每个结构体保存进 vector,每个结构体都是一篇文章了
        RssR.push(rss_item);
        
        item = item->NextSiblingElement();//找下一个item结点
    }

}


int main(){
    //test0();
    //test1();
    test2();
    return -1;
}
