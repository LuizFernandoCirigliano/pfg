#if !defined(NAO_CONST_DEF)
#define NAO_CONST_DEF

const char* robotName = "NAO";

const int headShapeCount = 2;
const char* headShapes[] = {
  "HeadYaw_link_respondable",
  "HeadPitch_link_respondable"
};

const int headJointCount = 2;
const char* headJoints[] = {
  "HeadYaw",
  "HeadPitch"
};

const int legShapeCount = 10;
const int legJointCount = 10;

const char* rightLegShapes[] = {
  "r_hip_roll_link_respondable3",
  "r_hip_roll_link_respondable5",
  "r_hip_pitch_pure2",
  "r_knee_pitch_link_pure2",
  "r_ankle_pitch_link_pure3",
  "r_sole_link_pure9",
  "r_sole_link_pure3",
  "r_sole_link_pure2",
  "r_sole_link_pure0",
  "r_sole_link_pure1",
};

const char* rightLegJoints[] = {
  "RHipYawPitch3",
  "RHipRoll3",
  "RHipPitch3",
  "RKneePitch3",
  "RAnklePitch3",
  "RAnkleRoll3",
  "NAO_RFsrRR",
  "NAO_RFsrFR",
  "NAO_RFsrRL",
  "NAO_RFsrFL",
};

const char *leftLegShapes[] = {
  "l_hip_roll_link_respondable3",
  "l_hip_roll_link_respondable4",
  "l_hip_pitch_pure2",
  "l_knee_pitch_link_pure2",
  "l_ankle_pitch_link_pure3",
  "l_ankle_link_pure3",
  "r_sole_link_pure5",
  "r_sole_link_pure6",
  "r_sole_link_pure7",
  "r_sole_link_pure8",
};

const char *leftLegJoints[] = {
  "LHipYawPitch3",
  "LHipRoll3",
  "LHipPitch3",
  "LKneePitch3",
  "LAnklePitch3",
  "LAnkleRoll3",
  "NAO_LFsrRR",
  "NAO_LFsrFR",
  "NAO_LFsrRL",
  "NAO_LFsrFL",
};

const int armJointCount = 5;
const int armShapeCount = 5;

const char *rightArmJoints[] = {
  "RShoulderPitch3",
  "RShoulderRoll3",
  "RElbowYaw3",
  "RElbowRoll3",
  "RWristYaw3"
};

const char *rightArmShapes[] = {
  "r_shoulder_pitch_respondable3",
  "r_shoulder_roll_link_respondable3",
  "r_elbow_yaw_link_respondable1",
  "r_elbow_roll_link_respondable3",
  "r_wrist_yaw_link_respondable3"
};

const char *leftArmJoints[] = {
  "LShoulderPitch3",
  "LShoulderRoll3",
  "LElbowYaw3",
  "LElbowRoll3",
  "LWristYaw3"
};

const char *leftArmShapes[] = {
  "r_shoulder_pitch_respondable3",
  "r_shoulder_roll_link_respondable3",
  "r_elbow_yaw_link_respondable1",
  "r_elbow_roll_link_respondable3",
  "r_wrist_yaw_link_respondable3"
};

#endif
