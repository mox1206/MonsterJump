//
//  GameCenter.h
//  MonsterJump
//
//  Created by 刘健 on 14-11-24.
//
//

#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>

@interface GameCenter : NSObject<GKLeaderboardViewControllerDelegate>
{
    BOOL gameCenterAvailable;
    BOOL userAuthenticated;
}

@property (assign, readonly) BOOL gameCenterAvailable;

+(GameCenter*) getInstance;
-(void) authenticaticateLocalUser;
-(BOOL) isGameCenterAvailable;
-(void) showLeaderboard;
-(void) leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController;

@end