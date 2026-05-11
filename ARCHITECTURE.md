# Multiplayer LAN System Architecture

## High-Level System Diagram

```
┌─────────────────────────────────────────────────────────┐
│                    SERVER (Port 8080)                   │
│                                                         │
│  ┌────────────────────────────────────────────────┐    │
│  │         Connection Acceptance Loop             │    │
│  │  - Listen on port 8080                        │    │
│  │  - Accept client connections                  │    │
│  │  - Spawn thread for each client               │    │
│  └────────────────────────────────────────────────┘    │
│                      ▲        ▲                         │
│            ┌─────────┴─────┬──┴─────────┐              │
│            │               │            │              │
│  ┌─────────▼──┐  ┌────────▼───┐  ┌───▼────────┐      │
│  │  Waiting   │  │  Active    │  │ Matchmake  │      │
│  │  Players   │  │  Matches   │  │ Logic      │      │
│  │  Queue     │  │            │  │            │      │
│  └────────────┘  └────────────┘  └────────────┘      │
│                                                         │
└─────────────────────────────────────────────────────────┘
         ▲              ▲              ▲
    ┌────┴──┐       ┌───┴──┐      ┌───┴──┐
    │        │       │       │      │       │
    
    
┌─────────────────────┐  ┌──────────────────────┐  ┌─────────────────────┐
│    CLIENT 1         │  │    CLIENT 2          │  │    CLIENT 3         │
│  (Player 1)         │  │  (Player 2)          │  │  (Waiting)          │
│                     │  │                      │  │                     │
│ ┌─────────────────┐ │  │ ┌──────────────────┐ │  │ ┌──────────────────┐│
│ │ Game Loop       │ │  │ │ Game Loop        │ │  │ │ Queue/Waiting    ││
│ ├─────────────────┤ │  │ ├──────────────────┤ │  │ ├──────────────────┤│
│ │ Typing Text     │ │  │ │ Typing Text      │ │  │ │ Connection:      ││
│ │ (Green bar)     │ │  │ │ (Green bar)      │ │  │ │ Waiting for      ││
│ │                 │ │  │ │                  │ │  │ │ opponent...      ││
│ │ Opponent:       │◄──┬──►│ Opponent:        │ │  │ │                  ││
│ │ (Blue bar)      │ │  │ │ (Blue bar)       │ │  │ │ [MATCH READY]    ││
│ │ [IN PROGRESS]   │ │  │ │ [IN PROGRESS]    │ │  │ │ ─────────────────││
│ │                 │ │  │ │                  │ │  │ │ Paired with      ││
│ │ [COMPLETED]     │ │  │ │ [COMPLETED]      │ │  │ │ CLIENT 1         ││
│ └─────────────────┘ │  │ └──────────────────┘ │  │ │ Starting match...││
│                     │  │                      │  │ └──────────────────┘│
└─────────────────────┘  └──────────────────────┘  └─────────────────────┘

         MATCH READY!         MATCH READY!              WAITING IN QUEUE
      Real-time Sync      Real-time Sync
```

## Network Communication Flow

```
PHASE 1: JOINING
═════════════════

CLIENT 1                    SERVER                     CLIENT 2
   │                           │                          │
   │──────JOIN_GAME───────────►│                          │
   │                           ├─ Queue player 1          │
   │                           │                          │
   │                           │◄─────JOIN_GAME──────     │
   │                           ├─ Queue player 2          │
   │                           ├─ Check queue size        │
   │                           ├─ SIZE >= 2! MATCH!       │
   │                           │                          │


PHASE 2: MATCH START
════════════════════

SERVER                      CLIENT 1    CLIENT 2
   │                           │            │
   ├──MATCH_START:text────────►│            │
   │                           │            │
   ├──MATCH_START:text────────────────────►│
   │                           │            │
   │ Both clients now have same text        │
   │ Match begins!                          │


PHASE 3: REAL-TIME GAMEPLAY
═════════════════════════════

CLIENT 1              SERVER              CLIENT 2
   │                    │                    │
   │ [User types: 'h']  │                    │
   │                    │                    │
   ├─PROGRESS:0.02 1 0─►│                    │
   │                    │                    │
   │                    ├─PROGRESS:0.02 1 0─┤
   │                    │                    │
   │ [User types: 'e']  │                    │
   │                    │                    │
   ├─PROGRESS:0.04 2 0─►│                    │
   │                    ├─PROGRESS:0.04 2 0─┤
   │                    │                    │
   │              [CLIENT 2 typing...]      │
   │                    │                    │
   │ [User continues]   │ [User types: 't']  │
   │                    │                    │
   ├─PROGRESS:0.08 4 0─►│                    │
   │                    ├─PROGRESS:0.08 4 0─┤
   │                    │                    │
   │                    ◄─PROGRESS:0.06 3 0─┤
   │ [Sees opponent at 3 chars]             │
   │                    │                    │


PHASE 4: COMPLETION
═══════════════════

CLIENT 1           SERVER          CLIENT 2
   │                  │               │
   │ [Completes!]     │               │
   │                  │               │
   ├─PROGRESS:1.0 50 1┤               │
   │                  │               │
   │                  ├─"COMPLETED!"─►│
   │                  │               │
   │ Sees opponent     │ [Still typing]│
   │ completed!        │               │
```

## Real-Time Display During Match

### Terminal Output

```
The quick brown fox jumps over the lazy dog

YOU:      |-------X----|    [Player 1's progress - green]
OPPONENT: |----X--------|    [Player 2's progress - blue]


Time: 5.23s

YOU:      |--------X---|    [Updated: more characters typed]
OPPONENT: |-----X------|    [Updated: opponent's progress]
```

## Thread Model

```
SERVER PROCESS
═══════════════════════════════════════════════════

Main Thread                      Worker Threads
│                                │
├─ while(true)                   │
│  └─ accept() connection         │
│     └─ spawn thread ────────────┼─► handleClient() #1
│                                │   - Read JOIN_GAME
│     └─ spawn thread ────────────┼─► handleClient() #2
│                                │   - Read JOIN_GAME
│                                │   - Trigger matchmake
│                                │
├─ Check waiting players list    │
│  └─ If 2+ players:             │
│     └─ Create match            │
│     └─ Send MATCH_START to both ├──► Both threads send
│                                │   to their clients
│                                │
│ ┌──────────────────────────────┼───────────────────┐
│ │ Player 1 Socket         Player 2 Socket         │
│ │ (in handleClient #1)    (in handleClient #2)    │
│ │   ▼                         ▼                    │
│ │ recv PROGRESS          recv PROGRESS             │
│ │   └─ Parse data          └─ Parse data          │
│ │   └─ Send to Player 2 ←─→ Send to Player 1     │
│ │                                                  │
│ │ recv GAME_END          recv GAME_END            │
│ │   └─ Mark match inactive                        │
│ │   └─ Cleanup resources                          │
│ └──────────────────────────────────────────────────┘


CLIENT PROCESS
══════════════════════════════════════════════════════

Main Thread                    Background Thread
│                              │
├─ Connect to server           │
├─ Send JOIN_GAME              │
├─ Receive MATCH_START:text    │
│                              │
├─ startMultiplayerSession()   │
│  ├─ while(!complete && !quit)│
│  │  ├─ Check keyboard (_kbhit)
│  │  ├─ Update UI display     │
│  │  │  └─ Show YOUR progress │
│  │  │  └─ Show OPPONENT bar  │ ◄── receiveGameUpdates()
│  │  │     (from bg thread)   │   - recv PROGRESS
│  │  │                        │   - Parse opponent data
│  │  │                        │   - Update opponentProgress
│  │  ├─ Send PROGRESS to srv  │
│  │  │  (after each keystroke)│
│  │  │                        │
│  │  └─ Render frame          │
│  │                           │
│  └─ Send GAME_END            │
│     └─ Game over             │
```

## Data Structures

### Server-Side

```cpp
struct GameMatch {
    int client1Socket;              // Socket for player 1
    int client2Socket;              // Socket for player 2
    std::string text;               // Match text (same for both)
    bool active;                    // Game status
};

struct PlayerMessage {
    int playerId;
    float progress;                 // 0.0 to 1.0
    int typedChars;                 // Number of correct chars
    bool completed;                 // Match finished?
};

class Server {
private:
    std::vector<int> waitingPlayers;        // Queue of clients waiting
    std::vector<GameMatch> activeMatches;   // Ongoing matches
    std::mutex playersMutex;                // Protect waiting queue
    std::mutex matchesMutex;                // Protect active matches
};
```

### Client-Side

```cpp
struct OpponentProgress {
    float progress;                 // 0.0 to 1.0
    int typedChars;
    bool completed;
};

class Client {
private:
    int clientSocket;
    std::string matchText;
    OpponentProgress opponentProgress;      // Current opponent state
    bool gameActive;
};
```

## Performance Characteristics

```
Latency: ~50-100ms (typical LAN)
├─ Network round trip
├─ Server processing
└─ Client update rendering

Update Rate: ~100-1000 Hz (per keystroke)
├─ Depends on typing speed
└─ No artificial frame limit

Memory: ~1KB per active match
├─ Socket + match data
└─ Scales linearly with player count

Throughput: ~100 bytes/sec per match
├─ Progress updates: ~40 bytes each
└─ ~2-10 updates per second typical

Max Concurrent: ~100+ matches
├─ Limited by file descriptors
└─ Threading overhead is minimal
```

## Security Notes

⚠️ **Current Implementation:**
- No encryption (plain text over network)
- No authentication (any client can join)
- No rate limiting
- No anti-cheat measures

✨ **For Production:**
- Add SSL/TLS encryption
- Implement user authentication
- Add client-side anti-cheat
- Rate limit progress updates
- Validate text boundaries
- Add admin panel for server management
