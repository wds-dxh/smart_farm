<template>
  <div>
    <!-- 新增的顶部标题 -->
    <div class="top-header">
      <h1> 智慧农场管理系统 </h1>
    </div>

    <!-- 左上角 -->
    <el-radio-group v-model="isCollapse" class="left_bt" style="margin-bottom: auto">
      <el-radio-button :value="false">展开</el-radio-button>
      <el-radio-button :value="true">折叠</el-radio-button>
    </el-radio-group>

    <!-- 左侧菜单 -->
    <el-menu
      default-active="1-1"
      class="left_menu"
      id="left_menu"
      :collapse="isCollapse"
      @open="handleOpen"
      @close="handleClose"
    >
      <!-- <el-sub-menu index="1">
        <template #title>
          <el-icon><location /></el-icon>
          <span>设备管理</span>
        </template>
        <el-sub-menu index="1-1">
          <template #title>设备：{{ deviceId }}</template>
          <el-menu-item index="1-1-1" @click="handleMenuItemClick('1-1-1')">设备控制&状态信息</el-menu-item>
        </el-sub-menu>
      </el-sub-menu> -->

      <el-sub-menu index="2">
        <template #title>
          <el-icon><location /></el-icon>
          <span> 状态监控 </span>
        </template>
        <el-menu-item index="2-1" @click="handleMenuItemClick('2-1')">状态信息</el-menu-item>
      </el-sub-menu>

      <el-menu-item index="3">
        <el-icon><Coordinate /></el-icon>
        <template #title>xxx</template>
      </el-menu-item>
      <el-menu-item index="4">
        <el-icon><Wallet /></el-icon>
        <template #title>xxx</template>
      </el-menu-item>
      <el-menu-item index="5">
        <el-icon><UserFilled /></el-icon>
        <template #title>xxx</template>
      </el-menu-item>
      <el-menu-item index="6">
        <el-icon><DArrowRight /></el-icon>
        <template #title>xxx</template>
      </el-menu-item>
      <el-menu-item index="7">
        <el-icon><Notebook /></el-icon>
        <template #title>xxx</template>
      </el-menu-item>
    </el-menu>

    <!-- 右侧内容区域 -->
    <div class="content">
      <DeviceControl v-if="currentView === 'device-control'" />
      <WorkshopStatus v-if="currentView === 'workshop-status'" />
    </div>
  </div>
</template>

<script lang="js" setup>
import { ref } from 'vue';
import DeviceControl from './DeviceControl.vue';
import WorkshopStatus from './WorkshopStatus.vue';

import {
  Menu as IconMenu,
  Location,
  Setting,
  Coordinate,
  Wallet,
  UserFilled,
  DArrowRight,
  Notebook
} from '@element-plus/icons-vue';

// 设备编号默认为1
const deviceId = ref(1);

const isCollapse = ref(false); // 左侧菜单是否折叠
const currentView = ref('workshop-status'); // 当前显示的视图

const handleOpen = (key, keyPath) => {
  console.log(key, keyPath);
};

const handleClose = (key, keyPath) => {
  console.log(key, keyPath);
};

const handleMenuItemClick = (index) => {
  if (index === '1-1-1') {
    currentView.value = 'device-control';
  } else if (index === '2-1') {
    currentView.value = 'workshop-status';
  } else {
    currentView.value = null;
  }
};
</script>

<style scoped>
.top-header {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  background-color: #409EFF;
  color: white;
  text-align: center;
  padding: 10px 0;
  z-index: 1000;
}

.top-header h1 {
  margin: 0;
  font-size: 20px;
}

.left_bt {
  position: absolute;
  top: 50px;
  left: 0;
  z-index: 999;
}

#left_menu {
  position: absolute;
  top: 90px;
  left: 0;
  z-index: 999;
}

.content {
  margin-left: 200px;
  padding: 20px;
}
</style>
