import Flow from '../Flow';
import Sidebar from '../Sidebar/Sidebar';
import ReactFlow, {
  ReactFlowProvider,
  useNodesState,
  useEdgesState,
  addEdge,
  Controls,
} from 'reactflow';

import './App.css';

function App() {
  return (
    <div className="App">
      <ReactFlowProvider>
        <header className="App-header">React Flow - CRA Example</header>
        <Flow />
        <Sidebar />
      </ReactFlowProvider>
    </div>
  );
}

export default App;
