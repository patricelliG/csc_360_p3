#include <stdio.h> 
 
extern struct rtpkt { 
  int sourceid;       /* id of sending router sending this pkt */ 
  int destid;         /* id of router to which pkt being sent  
                         (must be an immediate neighbor) */ 
  int mincost[4];    /* min cost to node 0 ... 3 */ 
  }; 
 
extern int TRACE; 
extern int YES; 
extern int NO; 
 
struct distance_table  
{ 
  int costs[4][4]; 
} dt0; 
 
 
/* students to write the following two routines, and maybe some others 
*/ 
struct rtpkt udpkt; // The update packet 

void rtinit0()  
{ 
    // Let the world know what we are up to...
    extern float clocktime;
    printf("executing rtinit0() at time: %10.5f\n", clocktime);

    // Initialize the routing table for node0
    // All distances should be initialized to "infinity"
    // where "infinity" is 999 as defined in the project spec
    int i, j;
    for (i = 0; i < 4; i++ )
    {
        for (j = 0; j < 4; j++)
        {
            dt0.costs[i][j] = 999;
        }
    }

    // Set known distances to neighboring nodes
    // where cost is the cost from neighbor i to node j
    dt0.costs[1][1] = 1;
    dt0.costs[2][2] = 3;
    dt0.costs[3][3] = 7;

    // Initialize all distances in the updating packet to infinity
    udpkt.mincost[0] = 999;
    udpkt.mincost[1] = 999;
    udpkt.mincost[2] = 999;
    udpkt.mincost[3] = 999;

    // Now compare the distance table values with the ones in the updating packet
    // if any distances are shorter, update the values in the updating packet
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            // Compare the distances
            if (udpkt.mincost[i] > dt0.costs[i][j])
            {
                // Update the distance in the packet
                udpkt.mincost[i] = dt0.costs[i][j];
            }
        }
    }

    // Now we are ready to send the initial packet from node 0

    // Update node 1
    udpkt.sourceid = 0; // This node's ID
    udpkt.destid = 1; // The destination node's ID
    tolayer2(udpkt); // Send it off...

    // Update node 2
    udpkt.sourceid = 0; // This node's ID
    udpkt.destid = 2; // The destination node's ID
    tolayer2(udpkt); // Send it off...

    // Update node 3
    udpkt.sourceid = 0; // This node's ID
    udpkt.destid = 3; // The destination node's ID
    tolayer2(udpkt); // Send it off...
} 
 
 
void rtupdate0(rcvdpkt) 
  struct rtpkt *rcvdpkt; 
{ 
    // Let the world know what we are up to...
    extern float clocktime;
    printf("executing rtupdate0() at time: %10.5f\n", 0.000);
    printf("update received from node: %d\n", rcvdpkt->sourceid); 


    // Initialize all distances in the updating packet to infinity
    udpkt.mincost[0] = 999;
    udpkt.mincost[1] = 999;
    udpkt.mincost[2] = 999;
    udpkt.mincost[3] = 999;

    // Check for any changes in the paths from the source to other nodes
    int i, j;
    int has_changed = 0; // If 0, nothing was updated, if 1, something changed
    int src = rcvdpkt->sourceid; // The sender's ID

    for (i = 0; i < 4; i++)
    {
        // Check if there is a shorter path from the sender of the received packet
        if ((rcvdpkt->mincost[i] + dt0.costs[src][src]) < dt0.costs[i][src])
        {
            // There is a shorter path, update this node's table
            dt0.costs[i][src] = (rcvdpkt->mincost[i] + dt0.costs[src][src]);
            // Note the change
            has_changed = 1;
        }
    }

    // If there was a change, update the other nodes!
    if (has_changed == 1)
    {
        // Show the update
        printf("routing table was updated");
        printdt0(&dt0);
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                if (dt0.costs[i][j] < udpkt.mincost[i])
                {
                    // Update the update packet
                    udpkt.mincost[i] = dt0.costs[i][j];
                }
            }
        }
        // Update neighboring nodes with new information
        printf("Updating neighboring nodes, 1, 2 and 3.\n");

        // Update node 1
        udpkt.sourceid = 0; // This node's ID
        udpkt.destid = 1; // The destination node's ID
        tolayer2(udpkt); // Send it off...

        // Update node 2
        udpkt.sourceid = 0; // This node's ID
        udpkt.destid = 2; // The destination node's ID
        tolayer2(udpkt); // Send it off...

        // Update node 3
        udpkt.sourceid = 0; // This node's ID
        udpkt.destid = 3; // The destination node's ID
        tolayer2(udpkt); // Send it off...
    }
} 
 
 
printdt0(dtptr) 
  struct distance_table *dtptr; 
   
{ 
  printf("                via     \n"); 
  printf("   D0 |    1     2    3 \n"); 
  printf("  ----|-----------------\n"); 
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1], 
  dtptr->costs[1][2],dtptr->costs[1][3]); 
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1], 
  dtptr->costs[2][2],dtptr->costs[2][3]); 
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1], 
  dtptr->costs[3][2],dtptr->costs[3][3]); 
} 
 
linkhandler0(linkid, newcost)    
  int linkid, newcost; 
 
/* called when cost from 0 to linkid changes from current value to 
newcost*/ 
/* You can leave this routine empty if you're an undergrad. If you want 
*/ 
/* to use this routine, you'll need to change the value of the 
LINKCHANGE */ 
/* constant definition in prog3.c from 0 to 1 */ 
  
{ 
} 
 

