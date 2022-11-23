#include"Rss.h"
RssReader::RssReader(){
    cout<<"RssReader"<<endl;
}

void RssReader::parseRss(const char*& FileName){

    XMLDocument xmlDocument;
    xmlDocument.LoadFile(FileName);
    XMLElement *category = xmlDocument.RootElement();
    XMLElement *channel = category->FirstChildElement("channel");//根节点第一个孩子channel
    //XMLElement *title = channel->FirstChildElement("title");//拿到channel的孩子结点title
    XMLElement *item = channel->FirstChildElement("item");//channel结点有很多个item子结点
    while(item)//遍历channel下的每个item结点
    {  
        RssItem rss_item;
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
        //cout<<rss_item.link<<endl;
 
		//获取desc结点指针，数据用正则去除html标签后，把内容保存到结构体相应的变量
        XMLElement *item_description = item->FirstChildElement("description");
        if(item_description)
        rss_item.description= item_description->GetText();
        //printf("333\n");
        //获取content结点指针，数据用正则去除html标签后，把内容保存到结构体相应的变量
        XMLElement *item_content = item->FirstChildElement("content:encoded");
        if(item_content)
            rss_item.content= item_content->GetText();
        //cout<<rss_item.content<<endl;
        
        //把每个结构体保存进 vector,每个结构体都是一篇文章了
        _rss.push_back(rss_item);    
        item = item->NextSiblingElement();//找下一个item结点
        
    }

  regex reg("<[^>]*>");
  //从容器中取出每一个item
  vector<RssItem>::iterator it=_rss.begin();
  //去除每一个字段的html标签
  for(;it!=_rss.end();++it){
      it->description=regex_replace(it->description,reg,"");
      it->link=regex_replace(it->link,reg,"");
      it->title=regex_replace(it->title,reg,"");
      it->content=regex_replace(it->content,reg,"");

  }
}
void RssReader::dump(const string &filename){
    //FILE* fw=fopen(filename,"w");
    std::ofstream fout;
    fout.open(filename);
    vector<RssItem>::iterator it=_rss.begin();
    for(int i=0;it!=_rss.end();++it,++i){
      for(int j=0;j<7;++j){fout<<"\t\t\t***"<<"ITEM"<<i<<"***"<<endl;}
        fout<<it->title<<endl;
        fout<<it->link<<endl;
        fout<<it->description<<endl;
        fout<<it->content<<endl;
      for(int j=0;j<7;++j){fout<<"\t\t\t***"<<"EOF"<<i<<"***"<<endl;}
    }
    fout.close();
}
