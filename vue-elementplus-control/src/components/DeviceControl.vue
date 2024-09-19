<template>
  <div class="device-control-wrapper">
    <div class="device-control">
      <h2>设备控制</h2>
      <el-form @submit.prevent="submitControl" label-width="200px">
        <el-form-item label="目标位置" :error="positionError">
          <el-input v-model="position" placeholder="请输入目标位置 (范围: 1-11)" clearable></el-input>
        </el-form-item>
        <el-form-item class="button-group">
          <el-button type="primary" @click="submitControl">发送</el-button>
          <el-button type="default" @click="resetForm">重置</el-button>
        </el-form-item>
      </el-form>
  
      <div class="device-status">
        <h2>设备状态</h2>
        <el-card class="status-card">
          <div class="status-item">
            <span class="status-label">位置:</span>
            <span class="status-value">{{ agvStatus.position }}</span>
          </div>
          <div class="status-item">
            <span class="status-label">状态:</span>
            <span :class="['status-value', agvStatus.status === '在线' ? 'online' : 'offline']">
              {{ agvStatus.status }}
            </span>
          </div>
          <div class="status-item">
            <span class="status-label">电池:</span>
            <span class="status-value">{{ agvStatus.battery }}%</span>
          </div>
          <div class="status-item">
            <span class="status-label">速度:</span>
            <span class="status-value">{{ agvStatus.speed }} cm/s</span>
          </div>
          <div class="status-item">
            <span class="status-label">最后更新:</span>
            <span class="status-value">{{ agvStatus.last_update }}</span>
          </div>
          <el-button type="primary" @click="fetchAgvStatus" class="refresh-button">刷新状态</el-button>
        </el-card>
      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios';
import { ref, onMounted } from 'vue';
import { ElNotification } from 'element-plus';

export default {
  name: 'DeviceControl',
  setup() {
    const position = ref('');
    const positionError = ref('');
    const agvStatus = ref({
      position: null,
      status: '离线',
      battery: null,
      speed: null,
      last_update: null
    });

    const submitControl = async () => {
      if (position.value === '') {
        ElNotification({
          title: '错误',
          message: '请输入目标位置',
          type: 'error',
        });
        return;
      }

      if (position.value < 1 || position.value > 11) {
        ElNotification({
          title: '提示',
          message: '目标位置的范围是1-11',
          type: 'warning',
        });
      }

      try {
        const response = await axios.post('http://localhost:5000/agv/control', 
          JSON.stringify({ position: position.value }), 
          {
            headers: {
              'Content-Type': 'application/json'
            }
          }
        );
        ElNotification({
          title: '成功',
          message: response.data.message,
          type: 'success',
        });
      } catch (error) {
        console.error('Error:', error);
        ElNotification({
          title: '错误',
          message: '发送控制命令失败',
          type: 'error',
        });
      }
    };

    const resetForm = () => {
      position.value = '';
    };

    const fetchAgvStatus = async () => {
      try {
        const response = await axios.post('http://localhost:5000/agv/status', 
          JSON.stringify({ detail: 'full' }), 
          {
            headers: {
              'Content-Type': 'application/json'
            }
          }
        );
        agvStatus.value = response.data;
        ElNotification({
          title: '成功',
          message: '获取设备状态成功',
          type: 'success',
        });
      } catch (error) {
        console.error('Error:', error);
        ElNotification({
          title: '错误',
          message: '获取设备状态失败',
          type: 'error',
        });
      }
    };

    onMounted(() => {
      fetchAgvStatus();
    });

    return {
      position,
      positionError,
      submitControl,
      resetForm,
      agvStatus,
      fetchAgvStatus,
    };
  },
};
</script>

<style scoped>
.device-control-wrapper {
  display: flex;
  justify-content: center;
  align-items: center;
  min-height: 100vh;
  background-color: #f5f5f5;
}

.device-control {
  width: 100%;
  max-width: 800px;
  padding: 40px;
  border: 1px solid #ebeef5;
  border-radius: 8px;
  background-color: #fff;
  box-shadow: 0 4px 20px 0 rgba(0, 0, 0, 0.1);
  transition: all 0.3s ease;
}

.device-control h2 {
  text-align: center;
  margin-bottom: 40px;
  color: #409EFF;
  transition: color 0.3s ease;
  font-size: 2em;
}

.el-form-item {
  margin-bottom: 30px;
  transition: margin-bottom 0.3s ease;
}

.el-input {
  width: 100%;
  transition: width 0.3s ease;
}

.button-group {
  display: flex;
  flex-direction: column;
  align-items: center;
}

.el-button {
  width: 200px;  /* 设置按钮宽度 */
  margin-top: 10px;
  transition: margin-top 0.3s ease;
}

.device-status {
  margin-top: 50px;
}

.device-status h2 {
  text-align: center;
  margin-bottom: 20px;
  color: #409EFF;
  transition: color 0.3s ease;
  font-size: 2em;
}

.status-card {
  padding: 20px;
  transition: padding 0.3s ease;
}

.status-item {
  display: flex;
  justify-content: space-between;
  margin-bottom: 10px;
}

.status-label {
  font-weight: bold;
  color: #333;
}

.status-value {
  color: #666;
}

.status-value.online {
  color: green;
}

.status-value.offline {
  color: red;
}

.refresh-button {
  display: block;
  margin: 20px auto 0;
}
</style>
