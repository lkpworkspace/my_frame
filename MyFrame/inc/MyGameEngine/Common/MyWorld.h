#ifndef MyWorld_H
#define MyWorld_H

typedef MyGameObj* (*MyGameObjCreateFunc)();

class MyWorld
{
public:

    static void StaticInit();

    static MyWorld* sInstance;

    /* 注册创建游戏对象的函数 */
    void RegisterCreationFunction( uint32_t inFourCCName, MyGameObjCreateFunc inCreationFunction );
    /* 创建游戏对象并将对象添加到世界中 */
    MyGameObj* CreateGameObject( uint32_t inFourCCName );

    /* 添加游戏对象到世界中 */
    void AddGameObject( MyGameObj* inGameObject );
    /* 将游戏对象从世界中删除 */
    void RemoveGameObject( MyGameObj* inGameObject );

    /* 更新这个世界中每一个游戏物体 */
    void Update();

private:
    MyWorld(){}
    std::vector< MyGameObj* >	mGameObjects;
    unordered_map< uint32_t, MyGameObjCreateFunc >	mNameToGameObjCreationFunctionMap;
};

#endif
