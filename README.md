This release is for assignment 4. Character animation

https://github.com/user-attachments/assets/fce9638a-9233-413b-afd6-34457ed934e8

There are 2 animation attach to player
- player_idle
- player_walk

I implement Animation Graph system where dev(me) can add and transition animation easily. Here is the example
```
	AnimationComponent& player_anim = game.animationStore.add(player);
	player_anim.Add("player_idle", game.assetManager.getAnimation("player_idle"));
	player_anim.Add("player_walk", game.assetManager.getAnimation("player_walk"));
	player_anim.AddBool("isWalking");
	player_anim.SetTrigger("player_idle", "player_walk", "isWalking", true);
	player_anim.SetTrigger("player_walk", "player_idle", "isWalking", false);
```
