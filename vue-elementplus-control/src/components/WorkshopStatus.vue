<template>
  <el-container class="workshop-status">
    <el-header class="header">
      <h2 class="title">大棚状态信息</h2>
    </el-header>
    <el-main class="main">
      <el-row :gutter="20" class="controls">
        <el-col :span="24">
          <el-form :inline="true" class="form-inline">
            <el-form-item label="查询记录数:" class="form-item">
              <el-input-number v-model="recordLimit" :min="1" :max="1000" size="large" />
            </el-form-item>
            <el-form-item class="form-item">
              <el-button type="primary" @click="fetchWorkshopData" size="large">查询</el-button>
            </el-form-item>
          </el-form>
        </el-col>
      </el-row>
      <el-row class="chart-controls">
        <el-col :span="24">
          <div class="radio-group">
            <el-radio-group v-model="selectedSeries">
              <el-radio label="温度">温度</el-radio>
              <el-radio label="湿度">湿度</el-radio>
              <el-radio label="照明度">照明度</el-radio>
            </el-radio-group>
          </div>
        </el-col>
      </el-row>
      <el-row>
        <el-col :span="24">
          <div ref="chart" class="chart"></div>
        </el-col>
      </el-row>

      <el-row :gutter="20" class="control-form">
        <el-col :span="24">
          <el-form :model="controlData" label-width="100px">
            <el-form-item label="水泵">
              <el-radio-group v-model="controlData.Water_pump">
                <el-radio label="on">开</el-radio>
                <el-radio label="off">关</el-radio>
              </el-radio-group>
            </el-form-item>
            <el-form-item label="加热">
              <el-radio-group v-model="controlData.heat">
                <el-radio label="on">开</el-radio>
                <el-radio label="off">关</el-radio>
              </el-radio-group>
            </el-form-item>
            <el-form-item label="照明度">
              <el-slider v-model="controlData.luminance" :min="0" :max="255" show-stops></el-slider>
            </el-form-item>
            <el-form-item>
              <el-button type="primary" @click="sendControlData">提交</el-button>
            </el-form-item>
          </el-form>
        </el-col>
      </el-row>
      
      <el-row class="video-controls">
        <el-col :span="24">
          <div class="video-container">
            <!-- <video id="video" controls autoplay>
              <source src="http://localhost:5001/video_feed">
              Your browser does not support the video tag.
            </video> -->
            <img src="http://localhost:5001/video_feed" width="640" height="480">
          </div>
        </el-col>
      </el-row>


    </el-main>
  </el-container>
</template>

<script>
import * as echarts from 'echarts';
import axios from 'axios';
import { ElNotification } from 'element-plus';

export default {
  name: 'WorkshopStatus',
  data() {
    return {
      chart: null,
      workshopData: [],
      recordLimit: 100, // 默认查询最近的 100 条记录
      selectedSeries: '温度', // 默认显示温度曲线
      controlData: {
        Water_pump: 'off',
        heat: 'off',
        luminance: 0,
      }
    };
  },
  mounted() {
    this.chart = echarts.init(this.$refs.chart);
    this.fetchWorkshopData();
  },
  watch: {
    selectedSeries() {
      this.renderChart();
    }
  },
  methods: {
    fetchWorkshopData() {
      axios.post('http://localhost:5000/smart_farm/status', { limit: this.recordLimit })
        .then(response => {
          this.workshopData = response.data;
          this.renderChart();
          this.showSuccessNotification('农场数据查询成功');
        })
        .catch(error => {
          console.error("There was an error fetching the workshop data!", error);
          this.showErrorNotification('农场数据查询失败');
        });
    },
    renderChart() {
      const temperatures = this.workshopData.map(item => item.temperatures);
      const humidnesses = this.workshopData.map(item => item.humidnesses);
      const illuminations = this.workshopData.map(item => item.illuminations);
      const timestamps = this.workshopData.map(item => item.timestamps);

      const series = [];

      if (this.selectedSeries === '温度') {
        series.push({
          name: '温度',
          type: 'line',
          data: temperatures,
          smooth: true,
          itemStyle: {
            color: '#FF6F61'
          }
        });
      }

      if (this.selectedSeries === '湿度') {
        series.push({
          name: '湿度',
          type: 'line',
          data: humidnesses,
          smooth: true,
          itemStyle: {
            color: '#6B8E23'
          }
        });
      }

      if (this.selectedSeries === '照明度') {
        series.push({
          name: '照明度',
          type: 'line',
          data: illuminations,
          smooth: true,
          itemStyle: {
            color: '#1E90FF'
          }
        });
      }

      const option = {
        title: {
          text: '农场状态信息',
          left: 'center'
        },
        tooltip: {
          trigger: 'axis'
        },
        legend: {
          data: [this.selectedSeries],
          top: '10%'
        },
        grid: {
          left: '3%',
          right: '4%',
          bottom: '3%',
          containLabel: true
        },
        xAxis: {
          type: 'category',
          data: timestamps
        },
        yAxis: {
          type: 'value'
        },
        series: series
      };

      this.chart.setOption(option);
    },
    sendControlData() {
      axios.post('http://localhost:5000/smart_farm/control', this.controlData)
        .then(response => {
          console.log(response.data);
          this.showSuccessNotification('控制数据发送成功');
        })
        .catch(error => {
          console.error("There was an error sending the control data!", error);
          this.showErrorNotification('控制数据发送失败');
        });
    },
    showSuccessNotification(message) {
      ElNotification({
        title: '成功',
        message: message,
        type: 'success',
        duration: 3000
      });
    },
    showErrorNotification(message) {
      ElNotification({
        title: '错误',
        message: message,
        type: 'error',
        duration: 3000
      });
    }
  }
};
</script>

<style scoped>
.workshop-status {
  padding: 20px;
  background-color: #f5f5f5;
  border-radius: 8px;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);
}

.header {
  text-align: center;
  margin-bottom: 20px;
}

.title {
  color: #333;
  font-size: 24px;
}

.controls {
  margin-bottom: 20px;
}

.form-inline {
  display: flex;
  align-items: center;
}

.form-item {
  margin-right: 20px;
}

.chart-controls {
  margin-bottom: 20px;
  text-align: center;
  border-bottom: 1px solid #dcdcdc;
  /* 增加分界线 */
  padding-bottom: 10px;
}

.radio-group {
  display: flex;
  justify-content: center;
  margin-bottom: 10px;
}

.chart {
  width: 100%;
  height: 400px;
  /* 减小图表的高度 */
  background-color: white;
  border-radius: 8px;
  padding: 20px;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);
}

.video-controls {
  margin-top: 20px;
  text-align: center;
}

.video-container {
  width: 100%;
  height: 480px;
  /* 设置视频容器的高度 */
  background-color: white;
  border-radius: 8px;
  padding: 20px;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);
}

video {
  width: 100%;
  height: 100%;
  border-radius: 8px;
}

.control-form {
  margin-top: 20px;
}

.control-form .el-form-item {
  margin-bottom: 20px;
}</style>
