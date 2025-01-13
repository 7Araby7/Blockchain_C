# Blockchain Simulation in C

## 📜 Description
This project is a simplified blockchain implementation for learning purposes. It simulates the creation, mining, and chaining of blocks in a blockchain structure. The application uses SHA-256 hashing, generates random bitcoin transactions, and organizes blocks in a linked list. An interactive menu allows users to query the blockchain for data like block hashes and address balances.

## 🚀 Features
- **Block Mining**: Implements mining logic using SHA-256 with adjustable difficulty.
- **Transaction Simulation**: Generates random transactions between 256 wallet addresses.
- **Linked List Management**: Dynamically stores mined blocks in a linked list.
- **Interactive Menu**:
  - Query block hashes.
  - Check wallet balances.
  - List wallets and their bitcoin amounts.
  - Identify the richest wallet.

## 🛠 Tools & Libraries
- **C**
- **OpenSSL**: For SHA-256 hashing.
- **MTwister**: For random number generation.

## 📂 Project Structure
- **`BlocoNaoMinerado`**: Represents a block before mining.
- **`BlocoMinerado`**: Represents a mined block with its hash and a pointer to the next block.
- **`menu()`**: Provides an interactive user interface for querying blockchain data.
- **`main()`**: Handles the blockchain initialization, mining process, and menu execution.

## 🧩 How It Works
1. **Initialize Wallets**: 256 wallet addresses are created, starting with a balance of zero.
2. **Generate Blocks**: Blocks are filled with transaction data and mined using SHA-256.
3. **Mining Difficulty**: Blocks are mined by adjusting the nonce until a hash meets the required difficulty.
4. **Interactive Menu**: Users can query the blockchain, check balances, and more.
