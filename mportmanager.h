#ifndef MPORTMANAGER_H
#define MPORTMANAGER_H

#include <qpair.h>
#include <qsharedpointer.h>
#include <qthread.h>
#include <qvariant.h>
#include <qvector.h>

#include <QByteArray>
#include <QHash>
#include <atomic>
#include <unordered_map>
//#include <QList>
#include <QMetaType>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QTimer>
#include <QWidget>
#include "iport.h"
#include <memory>

#include "order.h"
struct cmdData {
  uint32_t cmd;
  QByteArray data;
};
Q_DECLARE_METATYPE(cmdData)
struct comVaild {
  QString com;
  QString sn;
};

struct queryInfo {
  QVector<uint16_t> DL_set;    //电流设定
  QVector<uint8_t> DL_Status;  //电流状态

  uint16_t Freq_set;         //频率设定
  uint16_t Burst_num;        // burst个数
  QVector<uint16_t> YS_set;  //延时设定

  uint16_t Freq_DA;        //功率幅值
  uint8_t Freq_DA_Status;  //功率幅值开关

  uint16_t tri_state_in;  //触发模式
  //延时3
  uint8_t JG_Run;         //激光启动/停止

  uint16_t MK_set2;         //脉宽2
  QVector<uint16_t> DL_JX;  //电流极限

  uint8_t alarm_in;    //告警信息
  uint8_t seed_state;  //种子锁定
  QVector<uint16_t> DL_GZ;
  //保留2bytes
  QVector<uint16_t> Amp_work;   // amp工作温度
  QVector<uint16_t> JTWD_work;  // 晶体工作温度
  //保留1bytes
  uint8_t QTSD1;                //腔体湿度
  uint16_t SLL;                 //水流量
  uint8_t Debug;                //调试状态
  uint16_t SHG_WD_set;          // SHG温度设定
  uint16_t THG_WD_set;          // THG温度设定
  uint16_t SHG_WD_work;         //工作温度
  uint16_t THG_WD_work;         // THG工作温度
  //保留byte1
  uint8_t QTSD2;  //腔体湿度2
  uint8_t MNSZ_status;
  uint8_t Power_INOUT_status;      //功率内外控状态
  uint16_t Power_INOUT_BF;         //功率内外控百分比
  uint16_t Power_red;              //红外功率
  uint16_t PSO_POD_version;        // PSO/PDO版本号
  uint8_t LaserSN[14];             // laser sn
  QVector<uint16_t> SEED_LD_set;   //种子LD设定
  QVector<uint16_t> Seed_LD_work;  //种子LD工作
  QVector<uint8_t> seed_LD_run;    //种子ld启动

  QVector<uint16_t> seed_T_set;   //种子T系列的设定值
  QVector<uint16_t> seed_T_work;  //种子T系列的工作值
  uint32_t PIN2_set;              //密码2设定
  uint8_t Warning1_switch;        //报警1开关
  // byte2保留
  QVector<uint16_t> SX_YS;  //时序延时
  QVector<uint16_t> XH_YS;  //消耗延时
  uint8_t FP0;              //分频0
  uint8_t POD_GATE;         // pod gate
  uint32_t pin1;            // pin1设定
  uint8_t Warning2_switch;  //报警2开关
  uint8_t QDNC_QDC;         // QDNC/QDC
  uint16_t F_max_set_out;
  uint16_t F_min_set_out;

  uint16_t Burst_max_set_out;
  uint16_t Burst_min_set_out;

  uint16_t BP;  //倍频
  // byte1
  uint8_t QT_WD1;  //
  // byte1
  uint8_t QT_WD2;           //腔体温度2
  uint8_t JG_run_times[4];  //激光运行时间
  uint8_t hard_version[4];  //硬件版本号
  //电流4 设定 启动 极限 消耗
  uint16_t seed_run_point;  //种子运行位置
  uint8_t m20_50;           // 20M/50M
  uint8_t Warning3_switch;  //报警3开关
  uint16_t freq_zb;         //频率正补
  uint16_t freq_fb;         //频率负
  uint8_t mode;
  uint16_t seed_run_times;  //种子运行时间

  //查询2
  QVector<uint16_t> SXMK;   //时序脉宽
  QVector<uint16_t> XHMK;   //消耗脉宽
  uint8_t FP1;              //分频1
  uint8_t FP2;              //分频2
  uint16_t PowerC;          //功率乘数
  uint16_t PowerPz;         //功率偏置
  uint8_t KG;               //开盖状态
  QVector<uint16_t> Power;  //功率
  uint16_t SL2;             //水流2

  uint16_t PowerCorrect;   //功率校正
  uint8_t Dangwei;         //档位
  uint8_t CorrectSwitch;   //校正开关
  uint8_t CorrectCmd;      //校正指令
  uint16_t CorrenctBurst;  //校正burst
  uint8_t CorrenctStatus;  //校正状态

  uint8_t CollectTime; //采样时间
  uint8_t SaveFlashEnableState; //存储flash开启关闭状态
  uint16_t slLimit1Set;    //水流1最小值设定
  uint16_t slLimit2Set;    //水流2最小值设定
  uint8_t  slzSet;         //水流校正正设定
  uint8_t  slfSet;         //水流校正负设定
  uint8_t  GateFre1Set;         //gate分频1设置
  uint8_t  GateFre2Set;         //gate分频2设置
  uint16_t Power_INOUT_BF_2;    //功率2内控百分比
  uint8_t Debug_2;          //调试2状态
};

Q_DECLARE_METATYPE(queryInfo)
class mportManager : public QObject {
  Q_OBJECT
 public:
  explicit mportManager(QObject *parent = nullptr);
  virtual ~mportManager();
  // int send(const uint32_t& cmd, const QByteArray& data);
  int resetPort(iport *p = 0);
  int saveFreqVer(uint8_t id);
  void updateTimes(int t = 1000);
  int getFreqVer();
  int connectPort(const QString &name, uint8_t type = 0);
  int connectTcp(QString ip, int port);
  bool tryConnect(QVector<QString> &coms, QMap<QString, QString> &valid);
  void sendDisconnect(QString name);
  void sendConnect(QString name);
  void refresh();
  void close();
  int sendQuery();
  bool isConnect();
  void setCancelTry();
  int send(const uint32_t &cmd, const QByteArray &data, bool isGetErr = true,
           bool isRe = false);
  iport *getSerial(const QString &name);
  int parseQuery1(queryInfo &info, const QByteArray &data);
  int parseQuery2(queryInfo &info, const QByteArray &data);
  static mportManager *instance();
  int times = 5;
 public slots:
  void receiveData(QString, cmdData);
  void receiveDataFromTcp(QString, cmdData);
  void timerSlot();
  void sendDataToSerial(const uint32_t &cmd, const QByteArray &data,
                        bool isRefresh, bool getErr, bool isPrint);
  void saveCmdData(QString, cmdData);
  void portErr(int err);
  void setLogin(const uint32_t &isLogin);
  uint32_t getLogin();
  bool getCureEnable();
  void setEnableCorr(int isenable);
 signals:
  void sendCmd(QString, cmdData);
  void sendInfo(QString, queryInfo, int);
  void sendIsOk(QString, bool);
  void sendCmdToPort(const uint32_t &cmd, const QByteArray &data,
                     bool isRefresh, bool getErr, bool isPrint);
  void error(QByteArray);
  void DisConnect(QString);
  void changePsoOrPod(uint8_t);
  void valid(QString name);
  void curePowerEnable(int isEnable);

 protected:
 public:
  QList<QPair<QString, QWidget *>> mWdtList;

 protected:
  std::atomic_bool mCacelTry;
  iport *mSendPort;
  int mId = -1;
  uint8_t mfreqVer = 0;
  QByteArray mQuery1;
  QByteArray mQuery2;
  std::atomic_bool mRefresh[2];
  QTimer *mTimer;
  int mTimers = 3000;
  QThread *mTimerThd;
  uint32_t mIsLogin = false;
  bool mIsEnableCorr = false;
  bool mTcpIsConnected = false;
};
#define mportMg mportManager::instance()
#endif  // MPORTMANAGER_H
