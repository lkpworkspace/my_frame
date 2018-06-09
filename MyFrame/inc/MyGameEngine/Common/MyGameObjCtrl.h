#ifndef __MyGameObjCtrl_H__
#define __MyGameObjCtrl_H__

/**
 * @brief The MyGameObjCtrl class
 *
 *      该类仅模拟游戏物体(2D/3D)的移动
 *      需要设置的属性：
 *      必须设置的：
 *          坐标系
 *          初始位置：在(0,0,0)~(1,1,1)世界中的位置vec3
 *          移动速度：(0~1)/s
 *      ECT_Rotation2D：
 *          初始旋转：暂未定义
 *          旋转速度：(0~2*PI)/s
 *          前进方向：vec3
 */
class MyGameObjCtrl : public MyGameObj
{
public:
    enum EReplicationState
    {
        ERS_LocalPos = 1 << 0,
        ERS_LinearSpeed = 1 << 1,

        ERS_AllState = ERS_LocalPos | ERS_LinearSpeed
    };

    /* 游戏物体控制类型 */
    enum ECtrlType
    {
        /* 前后左右都是位移 */
        ECT_Normal2D,
        /* 左右是旋转， 前后是位移 */
        ECT_Rotation2D
    };

    MyGameObjCtrl();
    virtual ~MyGameObjCtrl(){}

    /**
     * 设置/获得坐标系 (目前没啥用)
     *      2D:
     *      屏幕左下角为坐标原点(0,0,0)
     *      屏幕左上角为坐标原点(0,1,0)
     *      屏幕中心为坐标原点(0.5f,0.5f,0)
     *      3D:
     *      无
     */
    void SetCoordinate(MyVec3& inCoordinate) { mCoordinate = inCoordinate; }
    MyVec3& GetCoordinate() { return mCoordinate; }

    void		SetPlayerId( uint32_t inPlayerId )			{ mPlayerId = inPlayerId; }
    uint32_t	GetPlayerId()						const 	{ return mPlayerId; }

    /* 设置/获得 物体位移类型 */
    void SetCtrlType(ECtrlType inCtrlType) { mCtrlType = inCtrlType; }
    ECtrlType GetCtrlType() { return mCtrlType; }

    /* 设置/获得 物体前进方向 */
    void SetForwardDir(MyVec3& inForwardDir) { mForwardDir = inForwardDir; }
    MyVec3& GetForwardDir() { return mForwardDir; }

    /* 设置/获得 物体直线速度 */
    void SetLinearSpeed(float inSpeed) { mMaxLinearSpeed = inSpeed; }
    float GetLinearSpeed() { return mMaxLinearSpeed; }

    /* 获得所有状态 */
    virtual uint32_t GetAllStateMask() const override { return ERS_AllState; }

    /* 处理 获得的数据 */
    virtual void ProcessInput( float inDeltaTime, const MyInputState& inInputState );
    /* 模拟移动 */
    virtual void SimulateMovement( float inDeltaTime );

    /* 设置/获得 物体速度 */
    void SetVelocity( const MyVec3& inVelocity )	{ mVelocity = inVelocity; }
    const MyVec3&	GetVelocity() const { return mVelocity; }

    virtual uint32_t	Write( MyOutputStream& inOutputStream, uint32_t inDirtyState ) const;
    virtual void		Read( MyInputStream& inInputStream );
protected:
    void ReadNormal2D( MyInputStream& inInputStream );
    void ReadRotation2D( MyInputStream& inInputStream );
    uint32_t WriteNormal2D(MyOutputStream& inOutputStream, uint32_t& inDirtyState ) const;
    uint32_t WriteRotation2D(MyOutputStream& inOutputStream, uint32_t& inDirtyState ) const;
private:
    void AdjustVelocityByThrust( float inDeltaTime );
    uint32_t mPlayerId;
    /* 设置坐标系类型 */
    MyVec3 mCoordinate;
    /* 物体控制类型 */
    ECtrlType mCtrlType;
    /* 前进方向 */
    MyVec3 mForwardDir;
    /* 速度(不知道干啥用) */
    MyVec3 mVelocity;
    /* 推力 */
    float mThrustDir;
    /* 最大直线速度 */
    float mMaxLinearSpeed;
    /* 最大旋转速度 */
    float mMaxRotationSpeed;
};

#endif
