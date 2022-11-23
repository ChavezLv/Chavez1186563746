/**
 * @file de_node.h
 * @brief Class Node manage interface
 * @author DeSdk Team
 * @version v1.0.0
 * @date 2020/03/13
 * @copyright Intellif
 */
 
#ifndef DE_NODE_H_H_
#define DE_NODE_H_H_

#include<string>
#include<functional>

#include "de_c_type.h"
#include "de_prop.h"

namespace de {

class PIN;	//class forward declaration

/**
 * @brief Holds and manages operator thread
 */
class DE_DLL Node {
public:
    Node(const std::string& node_class, const std::string& node_name, int dev_id, bool locate_remote = false);
    ~Node();
    void Start(void);
    void Stop(void);

    // 属性读写
    bool Set(const std::string& key, int val);
    bool Set(const std::string& key, double val);
    bool Set(const std::string& key, const std::string& val);
    void UpdatePara();

    bool IsProfileEanble(void);

    /**
     * @brief 判断Node Start时创建的Thread是否仍在运行
     */
    bool IsThreadActive(void);

    // Pin和Pout, 注意：PinBridge和PoutBridge表示通过Bridge中转过来的
    void* GetPin();
    void* GetPout(int idx);
    uint64_t GetPinQueue();
    void SetPoutQueue(int idx, uint64_t queue);

    std::string GetPoutTypeInfo(int idx);
    bool GetPoutLinkedFlag(int idx);
    std::string GetPoutName(int idx);
    std::string GetPinName(void);
    int GetTaskTypeId(const std::string& type_info);
    void SetTaskTypeId(int idx, int task_type_id);

    int SetPoutBridge(int pout_idx, void* p2p_chan);
    int SetPinBridge(int task_type);

    void SetDatabase(void* pdb);

    // source node's pout (pout_idx) link to destination node's pin
    int Link(int pout_idx, Node* dst);

    /**
     * @brief 获取Node状态
     * @return Node状态
     */
    int32_t GetDeState(void);

    /**
     * @brief 获取属性列表
     * @param[out] 用于存储属性列表的Prop对象
     */
    void GetAttrList(Prop& prop);

    /**
     * @brief 获取类名
     * @return 类名
     */
    std::string GetClassName(void) const { return node_class_; }

    /**
     * @brief source node's pout link to a pin
     * @param pout_idx pout index
     * @param ppin pointer to a pin
     * @note ppin must be local
     */
    int Link(int pout_idx, PIN* ppin);

    int dev_id_ = -1; // -1表示在host端或者local本地

    std::string node_name_;

private:
    std::string node_class_;
    
    void* handle_ = nullptr;
    
    bool para_updated_ = false;
};


/**
 * @brief Registers and manages node class information
 */
class NodeClassRegistry {
public:
    DE_DLL static NodeClassRegistry& Register(const std::string& name,
        const std::function<void* (void)>& create,
        const std::function<void (void*)>& destroy);

    DE_DLL void* Create();
    DE_DLL void  Destroy(void* p);
    DE_DLL static NodeClassRegistry* Get(const std::string &name);
    DE_DLL static bool Remove(const std::string& name);
    DE_DLL static void ListNames(void);

    struct ClassManager;

private:
    std::string class_name_;
    std::function<void* (void)> fun_create_;
    std::function<void (void*)> fun_destroy_;
    friend struct ClassManager;
};

#define DE_CLASS_REG_VAR_DEF                                        \
  static DE_ATTRIBUTE_UNUSED de::NodeClassRegistry& __mk_ ## DE


#define DE_CLASS_REGISTER(OpName, Type)                             \
  DE_STR_CONCAT(DE_CLASS_REG_VAR_DEF, __COUNTER__) =                \
      de::NodeClassRegistry::Register(OpName,                                \
        [](void) -> void* {return new Type; },                      \
        [](void* p) -> void {delete static_cast<Type*>(p); }        \
      )

}//end of namespace de

#endif
