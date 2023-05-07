import React from 'react';

export default () => {
  const onDragStart = (event: any, nodeType: any) => {
    event.dataTransfer.setData( 'application/reactflow' , nodeType);
    event.dataTransfer.effectAllowed = 'move' ;
  };

  return (
     <aside>
      <div className="description">Drag and drop nodes to the canvas to add</div>
      < div
        className = "dndnode input" 
        onDragStart ={(event) => {
          event. stopPropagation();
          onDragStart(event, 'input' );
        }}
        draggable
      >
        start node
      </div>
      < div
        className = "dndnode" 
        onDragStart ={(event) => {
          event. stopPropagation();
          onDragStart(event, 'custom' ); // The type is a custom node type name
        }}
        draggable
      >
        middle node
      </div>
      < div
        className = "dndnode output" 
        onDragStart ={(event) => {
          event. stopPropagation();
          onDragStart(event, 'output' );
        }}
        draggable
      >
        end node
      </div>
    </aside>
  );
};