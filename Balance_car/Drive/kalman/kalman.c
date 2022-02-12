#include "kalman.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "mpu6050.h"

#define sample_num 		100			//用于循环，100次
#define meas_interval	10			//延时10ms
#define	MPU_gRes		131.072f	//根据陀螺仪传感器设置,±250dps,  2^16/500=131.072

short accX, accY, accZ;				//加速度原始数据
short gyroX, gyroY, gyroZ;			//角速度原始数据

float theta_deg, theta_deg2;		//测量角度     卡尔曼滤波后的角度
float theta_dot, theta_dot2;		//测量角速度   减去偏差的角速度
 
float theta_mean;					//角度平均值
float theta_variance;				//角度方差
float theta_dot_mean;				//角速度平均值
float theta_dot_variance;			//角速度方差

//=========================================================
//卡尔曼滤波器的变量
//=========================================================
//卡尔曼滤波 2.5msec(400Hz)每次执行
const float theta_update_freq = 400; 									//400Hz
const float theta_update_interval = 1.0/(double)theta_update_freq; 		//2.5msec

//状态向量的预测值
float theta_data_predict[2][1];
float theta_data[2][1];

//共分散行列
float P_theta_predict[2][2];
float P_theta[2][2];

//状态方程式的"A"
float A_theta[2][2] = {{1, -theta_update_interval}, {0, 1}};
//状态方程式的"B"
float B_theta[2][1] = {{theta_update_interval}, {0}};
//输出方程式的"C"
float C_theta[1][2] = {{1, 0}};

//=========================================================
// 矩阵运算
//=========================================================
//矩阵的和
void mat_add(float *m1, float *m2, float *sol, int row, int column) {
    for(int i=0; i<row; i++) {
        for(int j=0; j<column; j++) {
            sol[i*column + j] = m1[i*column + j] + m2[i*column + j];    
        }    
    }
    return;
}

//矩阵的差
void mat_sub(float *m1, float *m2, float *sol, int row, int column){
    for(int i=0; i<row; i++) {
        for(int j=0; j<column; j++) {
            sol[i*column + j] = m1[i*column + j] - m2[i*column + j];    
        }    
    }
    return;
}

//矩阵的积
void mat_mul(float *m1, float *m2, float *sol, int row1, int column1, int row2, int column2){
    for(int i=0; i<row1; i++){
        for(int j=0; j<column2; j++){
            sol[i*column2 + j] = 0;
            for(int k=0; k<column1; k++) {
                sol[i*column2 + j] += m1[i*column1 + k]*m2[k*column2 + j];    
            }
        }    
    }
    return;
}

//转置矩阵
void mat_tran(float *m1, float *sol, int row_original, int column_original) {
    for(int i=0; i<row_original; i++) {
        for(int j=0; j<column_original; j++) {
            sol[j*row_original + i] = m1[i*column_original + j];    
        }    
    }
    return;
}

//矩阵的幂
void mat_mul_const(float *m1,float c, float *sol, int row, int column){
    for(int i=0; i<row; i++){
        for(int j=0; j<column; j++){
            sol[i*column + j] = c * m1[i*column + j];    
        }    
    }
    return;
}

//逆矩阵
void mat_inv(float *m, float *sol, int column, int row){
    //allocate memory for a temporary matrix
    float* temp = (float *)malloc( column*2*row*sizeof(float) );
    
    //make the augmented matrix
    for(int i=0; i<column; i++) {
        //copy original matrix
        for(int j=0; j<row; j++) {
            temp[i*(2*row) + j] = m[i*row + j];  
        }
        
        //make identity matrix
        for(int j=row; j<row*2; j++) {
            if(j-row == i) {
                temp[i*(2*row) + j] = 1;
            }    
            else {
                temp[i*(2*row) + j] = 0;    
            }
        }
    }

    //Sweep (down)
    for(int i=0; i<column; i++) {
        //pivot selection
        float pivot = temp[i*(2*row) + i];
        int pivot_index = i;
        float pivot_temp;
        for(int j=i; j<column;j++) {
            if( temp[j*(2*row)+i] > pivot ) {
                pivot = temp[j*(2*row) + i];
                pivot_index = j;
            }    
        }  
        if(pivot_index != i) {
            for(int j=0; j<2*row; j++) {
                pivot_temp = temp[ pivot_index * (2*row) + j ];
                temp[pivot_index * (2*row) + j] = temp[i*(2*row) + j];
                temp[i*(2*row) + j] = pivot_temp;    
            }    
        }
        
        //division
        for(int j=0; j<2*row; j++) {
            temp[i*(2*row) + j] /= pivot;    
        }
        
        //sweep
        for(int j=i+1; j<column; j++) {
            float temp2 = temp[j*(2*row) + i];
            
            //sweep each row
            for(int k=0; k<row*2; k++) {
                temp[j*(2*row) + k] -= temp2 * temp[ i*(2*row) + k ];    
            }    
        }
    }
        
    //Sweep (up)
    for(int i=0; i<column-1; i++) {
        for(int j=i+1; j<column; j++) {
            float pivot = temp[ (column-1-j)*(2*row) + (row-1-i)];   
            for(int k=0; k<2*row; k++) {
                temp[(column-1-j)*(2*row)+k] -= pivot * temp[(column-1-i)*(2*row)+k];    
            }
        }    
    }     
    
    //copy result
    for(int i=0; i<column; i++) {
        for(int j=0; j<row; j++) {
            sol[i*row + j] = temp[i*(2*row) + (j+row)];    
        }    
    }
    free(temp);
    return;
}

//====================================================
//卡尔曼滤波预处理
//====================================================
//从加速度传感器获取倾斜数据 单位：[deg]
float get_acc_data(void) {

  MPU_Get_Accelerometer(&accX,&accY,&accZ);
  theta_deg = atan2(-accY, accZ) * 57.29578f;
  
  return theta_deg;
}

//通过加速度传感器测量倾斜变化
void acc_init(void){
  float theta_array[sample_num];
  for(int i=0; i<sample_num; i++) {
    theta_array[i] = get_acc_data();
    delay_ms(meas_interval);
  }

  //平均值
  theta_mean = 0;
  for(int i=0; i<sample_num; i++) {
    theta_mean += theta_array[i];
  }
  theta_mean /= sample_num;
    
  //分散（方差）
  float temp;
  theta_variance = 0;
  for(int i=0; i<sample_num; i++) {
    temp = theta_array[i] - theta_mean;
    theta_variance += temp*temp;
  }
  theta_variance /= sample_num;
}

//求X轴角速度
float get_gyro_data(void) {

  MPU_Get_Gyroscope(&gyroX,&gyroY,&gyroZ);
  theta_dot = gyroX / MPU_gRes;
  
  return theta_dot;
}

//陀螺仪传感器偏差测量
void gyro_init(void) {
  float theta_dot_array[sample_num];
  for(int i=0;i<sample_num;i++) {
    theta_dot_array[i] =  get_gyro_data();
    delay_ms(meas_interval);
  }
    
  //平均值
  theta_dot_mean = 0;
  for(int i=0;i<sample_num;i++) {
    theta_dot_mean += theta_dot_array[i];    
  }
  theta_dot_mean /= sample_num;
 
  //分散
  float temp;
  theta_dot_variance = 0;
  for(int i=0; i<sample_num; i++) {
    temp = theta_dot_array[i] - theta_dot_mean;
    theta_dot_variance += temp*temp;    
  }
  theta_dot_variance /= sample_num;
}

//上电前的初始值运算
void ini_theta(void){
	//传感器偏差取得测量100次导出分散
	acc_init();
	gyro_init();

	//卡尔曼过滤器的初始设定假定初始姿势为0°（直立）
	theta_data_predict[0][0] = 0;
	theta_data_predict[1][0] = theta_dot_mean;

	P_theta_predict[0][0] = 1;
	P_theta_predict[0][1] = 0;
	P_theta_predict[1][0] = 0;
	P_theta_predict[1][1] = theta_dot_variance;
}

//=========================================================
//卡尔曼滤波处理
//=========================================================
void update_theta(void)
{     
    //加速度传感器角度测量
    float y = get_acc_data(); //degree
    
    //输入数据:角速度
    float theta_dot_gyro = get_gyro_data(); //degree/sec
      
    //计算卡尔曼增益: G = P'C^T(W+CP'C^T)^-1
    float P_CT[2][1] = {0};
    float tran_C_theta[2][1] = {0};
    mat_tran(C_theta[0], tran_C_theta[0], 1, 2);//C^T
    mat_mul(P_theta_predict[0], tran_C_theta[0], P_CT[0], 2, 2, 2, 1);//P'C^T
    float G_temp1[1][1] = {0};
    mat_mul(C_theta[0], P_CT[0], G_temp1[0], 1,2, 2,1);//CP'C^T
    float G_temp2 = 1.0f / (G_temp1[0][0] + theta_variance);//(W+CP'C^T)^-1
    float G1[2][1] = {0};
    mat_mul_const(P_CT[0], G_temp2, G1[0], 2, 1);//P'C^T(W+CP'C^T)^-1
    
    //倾斜角推算: theta = theta'+G(y-Ctheta')
    float C_theta_theta[1][1] = {0};
    mat_mul(C_theta[0], theta_data_predict[0], C_theta_theta[0], 1, 2, 2, 1);//Ctheta'
    float delta_y = y - C_theta_theta[0][0];//y-Ctheta'
    float delta_theta[2][1] = {0};
    mat_mul_const(G1[0], delta_y, delta_theta[0], 2, 1);
    mat_add(theta_data_predict[0], delta_theta[0], theta_data[0], 2, 1);
           
    //协方差矩阵计算: P=(I-GC)P'
    float GC[2][2] = {0};
    float I2[2][2] = {{1,0},{0,1}};
    mat_mul(G1[0], C_theta[0], GC[0], 2, 1, 1, 2);//GC
    float I2_GC[2][2] = {0};
    mat_sub(I2[0], GC[0], I2_GC[0], 2, 2);//I-GC
    mat_mul(I2_GC[0], P_theta_predict[0], P_theta[0], 2, 2, 2, 2);//(I-GC)P'
      
    //下一时刻倾斜角推算: theta'=Atheta+Bu
    float A_theta_theta[2][1] = {0};
    float B_theta_dot[2][1] = {0};
    mat_mul(A_theta[0], theta_data[0], A_theta_theta[0], 2, 2, 2, 1);//Atheta
    mat_mul_const(B_theta[0], theta_dot_gyro, B_theta_dot[0], 2, 1);//Bu
    mat_add(A_theta_theta[0], B_theta_dot[0], theta_data_predict[0], 2, 1);//Atheta+Bu 
    
    //下一时刻协方差矩阵计算: P'=APA^T + BUB^T
    float AP[2][2] = {0};   
    float APAT[2][2] = {0};
    float tran_A_theta[2][2] = {0};
    mat_tran(A_theta[0], tran_A_theta[0], 2, 2);//A^T 
    mat_mul(A_theta[0], P_theta[0], AP[0], 2, 2, 2, 2);//AP
    mat_mul(AP[0], tran_A_theta[0], APAT[0], 2, 2, 2, 2);//APA^T
    float BBT[2][2];
    float tran_B_theta[1][2] = {0};
    mat_tran(B_theta[0], tran_B_theta[0], 2, 1);//B^T
    mat_mul(B_theta[0], tran_B_theta[0], BBT[0], 2, 1, 1, 2);//BB^T
    float BUBT[2][2] = {0};
    mat_mul_const(BBT[0], theta_dot_variance, BUBT[0], 2, 2);//BUB^T
    mat_add(APAT[0], BUBT[0], P_theta_predict[0], 2, 2);//APA^T+BUB^T
	
	theta_deg2 = theta_data[0][0];
	theta_dot2 = theta_dot_gyro - theta_data[1][0];
}
