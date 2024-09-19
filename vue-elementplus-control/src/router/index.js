import { createRouter, createWebHistory } from 'vue-router';
import Dashboard from '../components/Dashboard.vue';
import Login from '../components/Login.vue';
import WorkshopStatus from '../components/WorkshopStatus.vue'; // 导入 WorkshopStatus 组件

const routes = [
  { path: '/', component: Login },
  { path: '/Dashboard', component: Dashboard },
  { path: '/workshop-status', component: WorkshopStatus } // 添加新的路由
];

const router = createRouter({
  history: createWebHistory(),
  routes
});

export default router;
