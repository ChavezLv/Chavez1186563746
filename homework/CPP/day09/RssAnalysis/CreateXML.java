	
    TiXmlDocument xmlDocument;

 
	// 添加XML声明
	xmlDocument.LinkEndChild(new TiXmlDeclaration( "1.0", "GBK", "" ));
 
	// 添加根元素
	TiXmlElement * xmlRoot = new TiXmlElement("root");
	xmlDocument.LinkEndChild(xmlRoot);
 
	//根元素下添加子元素1
	TiXmlElement* xmlChild1 = new TiXmlElement("name");
	xmlRoot->LinkEndChild(xmlChild1);
	xmlChild1->LinkEndChild(new TiXmlText("woniu"));
	xmlChild1->SetAttribute("id", "0001");//设置属性
 
 
	//根元素下添加子元素2
	TiXmlElement* xmlChild2 = new TiXmlElement("Student");
	xmlRoot->LinkEndChild(xmlChild2);
 
	TiXmlElement* xmlChild2_01 = new TiXmlElement("name");
	xmlChild2->LinkEndChild(xmlChild2_01);
	xmlChild2_01->LinkEndChild(new TiXmlText("woniu201"));
    
	TiXmlElement* xmlChild2_02 = new TiXmlElement("classes");
	xmlChild2->LinkEndChild(xmlChild2_02);
	xmlChild2_02->LinkEndChild(new TiXmlText("86"));
 
	//保存xml文件
	xmlDocument.SaveFile("woniu.xml");

